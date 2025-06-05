#pragma once
#include <memory>
#include <ESP32Encoder.h>

enum class State {
    READY,
    SPINNING,
    STOPPED,
    DISPENSING,
};

class WheelOfDestiny {
  ESP32Encoder* encoder;
  State state;

  static IRAM_ATTR void countChanged(void* arg) {
      ESP32Encoder* encoder = static_cast<ESP32Encoder*>(arg);
//      status = State::SPINNING;
  }

  public:
    WheelOfDestiny(int pinA, int pinB) : encoder(new ESP32Encoder(true, WheelOfDestiny::countChanged, static_cast<void*>(this))), state(State::READY) {
      encoder->attachSingleEdge(pinA, pinB);
    }
    // Encoder set up externally
    WheelOfDestiny(ESP32Encoder* encoder, int pinA, int pinB) : encoder(encoder), state{State::READY} {
    };

    State getState() { return state; };
    int getDrink();

    ~WheelOfDestiny() {
      if (encoder != nullptr) {
        if (encoder->isAttached())
          encoder->detach();
      }
    }


};
