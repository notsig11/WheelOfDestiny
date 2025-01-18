#include <Arduino.h>
#include <ESP32Encoder.h>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <Wire.h>
#include "i2c_proto.h"
#include "RecipeBook.h"
//#include <TFT_eSPI.h>

using namespace std::chrono_literals;

// Peripheral pins
#define SDA    2
#define SCL    1
#define OUT_A 12
#define OUT_B 11
#define RESET 17

constexpr uint8_t DRINK_SLOTS {30};
constexpr uint16_t COUNTS_PER_REV {600}; // Inside an 18" wheel ~ 5.72 * 600 counts.
constexpr uint16_t COUNTS_PER_DRINK = COUNTS_PER_REV / DRINK_SLOTS;

TwoWire i2c {0};
RecipeBook *recipeBook;

constexpr auto spinCompleteWait {1s};

int32_t lastValue {0};
volatile bool resetTriggered {false};

extern "C" {
int _write(int fd, char *ptr, size_t len) {
    (void) fd;
    return Serial.write(ptr, len);
}
}

enum class State {
    IDLE,
    SPINNING,
    STOPPED,
    DISPENSING,
};

volatile State status {State::IDLE};

// ISRs
static IRAM_ATTR void countChanged(void* arg) {
    ESP32Encoder* encoder = static_cast<ESP32Encoder*>(arg);
    status = State::SPINNING;
    if (resetTriggered) {
        resetTriggered = false;
        encoder->setCount(0);
    }
}

static IRAM_ATTR void resetEnc() {
    resetTriggered = true;
}

ESP32Encoder wheel { true, countChanged};
//TFT_eSPI display = TFT_eSPI();         // Declare object "tft"

void setup() {
    Serial.begin(115200);

    // I2C setup (master, 400kHz)
    i2c.begin(SDA, SCL);
    recipeBook = new RecipeBook(&i2c);

    // Wheel setup
    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    wheel.attachSingleEdge(OUT_A, OUT_B);

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


/*
 * States:
 *      idle -> spinning --5s stopped --> stopped spinning (make drink) --done dispensing -> idle.
 */
std::chrono::time_point<std::chrono::steady_clock> stoppedAtTime;
void loop() {
    const int32_t position = abs(static_cast<int32_t>(wheel.getCount()));

    // if (resetTriggered) {
    //     Serial.printf("Reset triggered at count %d\n", position);
    //     resetTriggered = false;
    //     wheel.setCount(0);
    // }

    // Wrap the counter?
//    if (position > COUNTS_PER_REV) {
//        wheel.setCount(position - COUNTS_PER_REV);
//        Serial.println("1 rev reset to " + String(position));
//    }

    // Real work... Check the counter is moving and change states based on that.
    if (lastValue != position) {
        if (status == State::DISPENSING) // Probably just stop the encoder read during dispensing...
            return;

        lastValue = position;
        if (status != State::SPINNING && status != State::DISPENSING) {
            Serial.printf("Transition from %d to %d\n", status, State::SPINNING);
            status = State::SPINNING;
            return;
        }
    } else { // Stopped...
        if (status == State::IDLE) return;
        // stopped.
        if (status == State::SPINNING) {
            Serial.printf("Stopped! Count %d (%d) (settling for 5s)\n", position, lastValue);
            stoppedAtTime = std::chrono::steady_clock::now();
            status = State::STOPPED;
        } else if (status == State::STOPPED) {
            if (spinCompleteWait < std::chrono::steady_clock::now() - stoppedAtTime) {
                const auto waited = std::chrono::steady_clock::now() - stoppedAtTime;
                if (status == State::DISPENSING) {
                    // Debugging TODO: REMOVEME when we get feedback that dispensing is complete...
                    status = State::IDLE;
                    Serial.println("Back to IDLE");
                    return;
                }
                status = State::DISPENSING;
                auto drinkIndex = position / COUNTS_PER_DRINK;
                const auto recipe = recipeBook->getRecipeAtIndex(drinkIndex);
                if (!recipe) {
                    status = State::IDLE;
                    Serial.printf("Can't find recipe %d\n", drinkIndex);
                    return;
                }
                std::cout << "Dispensing " << recipe->name << "[" << drinkIndex << "] (encoder position " << position << ", waited " << waited.count() / 1'000'000 << "ms)\n";

                if (!recipeBook->mixRecipe(recipe->name)) {
                    std::cout << "Failed at dispensing... \n";
                    status = State::IDLE;
                }
                delay(1000);
            }
        }
    }
    delay(100);
}
