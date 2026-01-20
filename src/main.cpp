#include <Arduino.h>
#include <ESP32Encoder.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <Wire.h>

#include "utils.h"
#include "state.h"
#include "config.h"
#include "i2c_proto.h"
#include "RecipeBook.h"
#include "Adafruit_VL53L0X.h"
//#include <TFT_eSPI.h>

using namespace std::chrono_literals;
extern int _write(int fd, char *ptr, size_t len);

// Global state vars
volatile int32_t lastValue {0};
volatile bool resetTriggered {false};
volatile State status {State::READY};
std::chrono::time_point<std::chrono::steady_clock> stoppedAtTime;
std::chrono::time_point<std::chrono::steady_clock> dispensingStartTime;

// Encoder changed interrupt.  Puts us in "SPINNING" if we are armed by a cup.
static IRAM_ATTR void countChanged(void* arg) {
    ESP32Encoder* encoder = static_cast<ESP32Encoder*>(arg);
    if (status == State::ARMED)
        status = State::SPINNING;
}

// 1 rev interrupt
static IRAM_ATTR void resetEnc() {
//    Serial.println("Reset");
    resetTriggered = true;
}

// Device globals
TwoWire i2c {0};
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
ESP32Encoder wheel { true, countChanged};
RecipeBook *recipeBook;

void setup() {
    Serial.begin(115200);

    // I2C setup (master, 400kHz)
    i2c.begin(SDA, SCL, 400'000);
    recipeBook = new RecipeBook(&i2c);

    // Cup sensor initialization.
    if (!lox.begin(VL53LOX_ADDR, true, &i2c)) {
        Serial.println(F("Failed to boot VL53L0X"));
    }
    lox.startRangeContinuous();

    // TODO: Move the display here?

    // Wheel setup
    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    wheel.attachSingleEdge(OUT_A, OUT_B);
    wheel.setFilter(1020); // PCNT Filter threshold

    // Reset switch/wheel homing.
    pinMode(RESET, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RESET), resetEnc, RISING);

    // Debug startup info.
    Serial.println("Startup configuration complete.");
    Serial.printf("Total heap: %d\n", ESP.getHeapSize());
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
    std::cout << "Initialization Complete... Find out your destiny.\n";
}

// Send commands to display handler.
void display_update(uint8_t message) {
    i2c.beginTransmission(DISPLAY_ADDRESS);
    i2c.write(message);
    i2c.endTransmission();
}

bool check_cup() {
    // Check for cup.
    if (lox.isRangeComplete()) {
        Serial.print("Distance in mm: ");
        const auto range = lox.readRange();
        // Distance to detect glass < 100 == glass in
        if (range > 100) {
            if (status == State::NO_CUP)
                return false;

            status = State::NO_CUP;
            i2c.beginTransmission(DISPLAY_ADDRESS);
            i2c.write(DISPLAY_NO_CUP);
            i2c.endTransmission();
            return false;
        } else {
            status = State::READY;
            return true;
        }
    }
    return false;
}

/*
 * States:
 *      idle -> spinning --5s stopped --> stopped spinning (make drink) --done dispensing -> idle.
 */
//std::vector<uint32_t> resets {};
//uint64_t sum {};
void loop() {
    const volatile int32_t position = abs(static_cast<int32_t>(wheel.getCount()));
    if (!check_cup())
        return;

//    Serial.printf("Current counts: %d.  Drink %d\n", position, position / COUNTS_PER_DRINK);

    if (resetTriggered) {
        resetTriggered = false;

        if (status == State::READY)
            status = State::ARMED;
        Serial.printf("Reset count %d\n", position);
//        resets.push_back(position);
//        std::for_each(resets.begin(), resets.end(), [&](int n) {sum += n;});
//        Serial.printf("Average reset counts: %d in %d resets\n", sum / resets.size(), resets.size());
        wheel.setCount(0);
        return;
    }

    // Real work... Check the counter is moving and change states based on that.
    // switch (status) {
    //     case State::NO_CUP:
    //         // TBD
    //         break;
    //     case State::READY:
    //         if (resetTriggered) {
    //             resetTriggered = false;
    //             status = State::ARMED;
    //             Serial.printf("Reset count %d\n", position);
    //             wheel.setCount(0);
    //         }
    //         break;
    //     case State::ARMED:
    //         if (lastValue != position) {
    //             status = State::SPINNING;
    //             i2c.beginTransmission(DISPLAY_ADDRESS);
    //             i2c.write(DISPLAY_SPINNING);
    //             i2c.endTransmission();
    //         }
    //         break;
    //     case State::SPINNING:
    //         if (lastValue == position) {
    //             stoppedAtTime = std::chrono::steady_clock::now();
    //             status = State::STOPPED;
    //             Serial.printf("Count %d (%d) (settling for 5s)\n", position, lastValue);
    //         }
    //         break;
    //     case State::STOPPED: {}
    //         if (lastValue != position) {
    //             status = State::SPINNING;
    //             return;
    //         }
    //
    //         if (spinCompleteWait < std::chrono::steady_clock::now() - stoppedAtTime) {
    //             status = State::DISPENSING;
    //             // dispense()
    //             auto drinkIndex = position / COUNTS_PER_DRINK;
    //             const auto recipe = recipeBook->getRecipeAtIndex(drinkIndex);
    //             if (!recipe) {
    //                 status = State::READY;
    //                 Serial.printf("Can't find recipe %d\n", drinkIndex);
    //                 return;
    //             }
    //             if (!recipeBook->mixRecipe(recipe->name)) {
    //                 std::cout << "Failed dispensing " << recipe->name << "\n";
    //                 status = State::READY;
    //             } else {
    //                 std::cout << "Dispensing " << recipe->name << "[" << drinkIndex << "]" << "\n";
    //             }
    //         }
    //         break;
    //     case State::DISPENSING: {
    //         const auto waited = std::chrono::steady_clock::now() - stoppedAtTime;
    //         if (waited > dispensingWait) {
    //             std::cout << "Dispensing for encoder position " << position
    //                       << ", wait complete in " << waited.count() / 1'000'000 << "ms)\n";
    //             status = State::READY;
    //             std::cout << "Back to READY state.\n";
    //         }
    //         break;
    //     }
    //     default:
    //         std::cout << "Unknown state " << status << "\n";
    // }

    if (lastValue != position) {
        Serial.printf("State: %d Pos changed... %d - %d\n", status, lastValue, position);
        lastValue = position;

        if (status == State::ARMED) {
            Serial.printf("Transition from %d to %d\n", status, State::SPINNING);
            status = State::SPINNING;
            display_update(DISPLAY_SPINNING);
            return;
        }
    } else {
        // Stopped...
        if (status == State::READY) return;

        if (status == State::SPINNING) {
            Serial.printf("Stopped! Count %d (%d) (settling for 5s)\n", position, lastValue);
            stoppedAtTime = std::chrono::steady_clock::now();
            status = State::STOPPED;
            display_update(DISPLAY_WAITING);
        }

        if (status == State::STOPPED) {
            if (const auto waited = std::chrono::steady_clock::now() - stoppedAtTime; spinCompleteWait < waited) {
                // TODO: Refactor to simplify this...
                status = State::DISPENSING;

                auto drinkIndex = position / COUNTS_PER_DRINK;
                const auto recipe = recipeBook->getRecipeAtIndex(drinkIndex);
                if (!recipe) {
                    status = State::READY;
                    Serial.printf("Can't find recipe %d\n", drinkIndex);
                    display_update(DISPLAY_LOST_RECIPE);
                    return;
                }

                std::cout << "Dispensing " << recipe->name << "[" << drinkIndex << "] (encoder position " << position
                          << ", waited " << waited.count() / 1'000'000 << "ms)\n";

                dispensingStartTime = std::chrono::steady_clock::now();
                if (!recipeBook->mixRecipe(recipe->name)) {
                    std::cout << "Failed at dispensing... \n";
                    status = State::READY;
                }
                return;
            }
        }
    }

    if (status == State::DISPENSING) {
        if (const auto waited = std::chrono::steady_clock::now() - dispensingStartTime; dispensingWait < waited) {
            // Debugging TODO: REMOVEME when we get feedback that dispensing is complete...  Don't forget the time check.
            status = State::READY;
            std::cout << "Waited for dispensing to complete for " << waited.count() / 1'000'000 << " ms.  Back to READY!\n";
            display_update(DISPLAY_READY);
            return;
        }
    }
    delay(100);
}