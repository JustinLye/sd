#pragma once

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <limits>
#include <set>

#include "sd/framework/interfaces/IGameClock.h"
#include "sd/framework/time/clock.h"
#include "sd/gameplay/time/timer.h"
#include "sd/gameplay/time/game_clock_timer.h"
#include "sd/gameplay/time/timer_status_t.h"

namespace sd {
namespace gameplay {
namespace time {
    class GameClockTimer;
    class GameClock :
        private sd::framework::time::Clock,
        public sd::framework::interfaces::IGameClock {
    public:
        GameClock();
        GameClock(const GameClock& other);
        GameClock(GameClock&& other) noexcept;
        ~GameClock();

        GameClock& operator=(const GameClock& other);
        GameClock& operator=(GameClock&& other) noexcept;

        virtual double DeltaTime() const noexcept override;
        bool IsPaused() const noexcept;

        void Tick();
        void Pause();
        void Resume();

        std::shared_ptr<GameClockTimer> StartTimer(double seconds);
        void CancelTimer(const Timer& timer);

        using Clock::Elapsed;

        inline friend std::ostream& operator<<(std::ostream& o, const GameClock& clock) {
            double fps = 0.0;
            if (clock.DeltaTime() > std::numeric_limits<double>::epsilon()) {
                fps = 1 / clock.DeltaTime();
            }
            o << "Elapsed Time: " << clock.Elapsed() << " Delta Time: " << clock.DeltaTime() << " FPS: " << fps;
            return o;
        }

    private:
        double m_ElapsedSecondsAtLastTick;
        double m_DeltaTime;
        std::mutex m_GameClockMutex;
        std::condition_variable m_GameClockCV;
        std::set<std::size_t> m_CancelledTimerIds;
        std::shared_ptr<GameClock> m_This;

        timer_status_t WaitFor(double seconds, std::size_t timer_id);
    };
}}}