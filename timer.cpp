#include "timer.h"

extern Timer attackTimer;
extern Timer jumpTimer;

Timer::Timer() : start(std::chrono::high_resolution_clock::now()), duration(0) {}

void Timer::set(std::chrono::milliseconds duration) {
    this->start = std::chrono::high_resolution_clock::now();
    this->duration = duration;
}

bool Timer::isExpired() const {
    return std::chrono::high_resolution_clock::now() - start >= duration;
}

void Timer::reset() {
    this->start = std::chrono::high_resolution_clock::now();
}
