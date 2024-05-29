#pragma once
#include <chrono>


class Timer {
public:
    Timer();
    void set(std::chrono::milliseconds duration);
    bool isExpired() const;
    void reset();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::milliseconds duration;
};
