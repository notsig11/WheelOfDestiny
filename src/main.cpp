#include <Arduino.h>
#include <ESP32Encoder.h>

#define OUT_A 13
#define OUT_B 10
#define RESET 17

ESP32Encoder wheel;
int32_t lastValue;

void setup() {
    Serial.begin(115200);
    ESP32Encoder::useInternalWeakPullResistors = puType::up;
    wheel.attachHalfQuad(13, 10);

    Serial.println("Startup configuration complete.");
}

void loop() {
    if (digitalRead(RESET)) {
        wheel.setCount(0);
        Serial.println("Reset/loop.");
    }

    if (lastValue != wheel.getCount()) {
        Serial.println("Encoder count = " + String(static_cast<int32_t>(wheel.getCount())));
        lastValue = wheel.getCount();
    }
    delay(100);
}