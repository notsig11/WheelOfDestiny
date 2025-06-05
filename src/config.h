#pragma once
#include <chrono>

using namespace std::chrono_literals;
// Peripheral pins
#define SDA    2
#define SCL    1
#define OUT_A 12
#define OUT_B 11
#define RESET 17

// Constants
constexpr uint8_t DRINK_SLOTS {30};
constexpr uint16_t COUNTS_PER_REV {4253}; // Inside an 18" wheel ~ 5.72 * 600 counts.
constexpr uint16_t COUNTS_PER_DRINK = COUNTS_PER_REV / DRINK_SLOTS;
constexpr auto spinCompleteWait {2s};
constexpr auto dispensingWait {15s}; // Total time to dispense and return to ready = spinCompleteWait + dispensingWait.
