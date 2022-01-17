#pragma once

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>

#include "sd/framework/time/clock.h"
#include "sd/gameplay/time/timer.h"
#include "sd/gameplay/time/timer_status_t.h"

namespace sd {
namespace gameplay {
namespace time {

    class GameClock :
        private sd::framework::time::Clock {
    public:
        GameClock();
        GameClock(const GameClock& other);
        GameClock(GameClock&& other) noexcept;
        ~GameClock();

        GameClock& operator=(const GameClock& other);
        GameClock& operator=(GameClock&& other) noexcept;

        double DeltaTime() const noexcept;
        bool IsPaused() const noexcept;

        void Tick();
        void Pause();
        void Resume();

        Timer StartTimer(double seconds);

        using Clock::Elapsed;

        inline friend std::ostream& operator<<(std::ostream& o, const GameClock& clock) {
            o << "Elapsed Time: " << clock.Elapsed() << " Delta Time: " << clock.DeltaTime() << " FPS: " << 1 / clock.DeltaTime();
            return o;
        }

    private:
        double m_ElapsedSecondsAtLastTick;
        double m_DeltaTime;
        std::mutex m_GameClockMutex;
        std::condition_variable m_GameClockCV;

        timer_status_t WaitFor(double seconds);
    };
}}}