#pragma once

#include <chrono>
#include <thread>
#include <iostream>

struct Timer
{
    using HighResClock = std::chrono::high_resolution_clock;
    std::chrono::time_point<std::chrono::steady_clock> startTime, endTime;

    std::chrono::duration<float> duration;
    float miliseconds;
    float seconds;
    
    void start() {
        startTime = HighResClock::now();
    }

    void end() {
        endTime = HighResClock::now();
        duration = endTime - startTime;
        miliseconds = duration.count() * 1000.0f;
        seconds = duration.count(); 
    }
};
