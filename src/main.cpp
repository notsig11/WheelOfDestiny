#include <Arduino.h>
#include <ESP32Encoder.h>
#include <chrono>
#include <stdio.h>
#include <iostream>
#include <Wire.h>
//#include <TFT_eSPI.h>

using namespace std::chrono_literals;

// Peripheral pins
#define OUT_A 13
#define OUT_B 10
#define RESET 17

constexpr uint8_t DRINK_SLOTS {30};
constexpr uint16_t COUNTS_PER_REV {600}; // Inside an 18" wheel ~ 5.72 * 600 counts.
constexpr uint16_t COUNTS_PER_DRINK = COUNTS_PER_REV / DRINK_SLOTS;

constexpr auto spinCompleteWait {5.0s};

int32_t lastValue;

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

State status {State::IDLE};

static IRAM_ATTR void countChanged(void* arg) {
    ESP32Encoder* encoder = static_cast<ESP32Encoder*>(arg);
    status = State::SPINNING;
}

ESP32Encoder wheel { true, countChanged};
//TFT_eSPI display = TFT_eSPI();         // Declare object "tft"

void setup() {
    Serial.begin(115200);
    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    wheel.attachSingleEdge(OUT_A, OUT_B);

    Serial.println("Startup configuration complete.");
    Serial.printf("Total heap: %d\n", ESP.getHeapSize());
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
    std::cout << "testing\n";
}

/*
 * States:
 *      idle -> spinning --5s stopped --> stopped spinning (make drink) --done dispensing -> idle.
 */
std::chrono::time_point<std::chrono::steady_clock> stoppedAtTime;
void loop() {
    int32_t position = abs(static_cast<int32_t>(wheel.getCount()));

    // Wrap the counter?
//    if (position > COUNTS_PER_REV) {
//        wheel.setCount(position - COUNTS_PER_REV);
//        Serial.println("1 rev reset to " + String(position));
//    }

    // 1 rev reset/check.
    if (digitalRead(RESET)) {
        if (status != State::IDLE)
            Serial.printf("Reset/loop.  Counter: %d\n", wheel.getCount());
        wheel.setCount(0);
        status = State::IDLE;
    }

    // Real work... Check the counter is moving and change states based on that.
    if (lastValue != position) {
        if (status == State::DISPENSING) // Probably just stop the encoder read during dispensing...
            return;

        lastValue = position;
        if (status != State::SPINNING && status != State::DISPENSING) {
            status = State::SPINNING;
            return;
        }
        // Still spinning.
    } else {
        if (status == State::IDLE) return;
        // stopped.
        if (status == State::SPINNING) {
            Serial.printf("Stopped! (settling for 5s)");
            stoppedAtTime = std::chrono::steady_clock::now();
            status = State::STOPPED;
            return;
        }
        if (spinCompleteWait < std::chrono::steady_clock::now() - stoppedAtTime) {
            const auto waited = std::chrono::steady_clock::now() - stoppedAtTime;
            if (status == State::DISPENSING) {
                // Debugging TODO: REMOVEME
                status = State::IDLE;
                return;
            }
            status = State::DISPENSING;
            std::cout << "Dispensing drink " << position / COUNTS_PER_DRINK << " (encoder position " << position << ", waited " << waited.count() / 1000000 << "ms)\n";
        } else {
            const auto waited = std::chrono::steady_clock::now() - stoppedAtTime;
            std::cout << "Stopped!  Final position: " << position << " Waiting to be sure... " << waited.count() / 1000000 << "ms\n";
        }
    }
    delay(100);
}