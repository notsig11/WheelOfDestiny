/*
 * State definitions... Tired of the huge block of junk in the main file.
 */
#pragma once
#include <initializer_list>
#include <iosfwd>
#include <utility>

enum class State {
    NO_CUP,
    READY,
    ARMED,
    SPINNING,
    STOPPED,
    DISPENSING,
};

constexpr std::initializer_list<std::pair<State, const char*>> stateMap = {
    {State::NO_CUP, "no cup"},  {State::READY, "ready"},   {State::ARMED, "armed"},
    {State::SPINNING, "wheel spinning"}, {State::STOPPED, "wheel stopped"}, {State::DISPENSING, "dispensing"}
};

std::ostream& operator<<(std::ostream& os, State s) {
    for (auto& state : stateMap) {
        if (s == state.first) {
            os << state.second;
            return os;
        }
    }
    os << "unknown state";
    return os;
}
