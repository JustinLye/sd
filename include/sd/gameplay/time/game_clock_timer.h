#pragma once

#include <memory>

#include "sd/gameplay/time/game_clock.h"
#include "sd/gameplay/time/timer.h"

namespace sd {
namespace gameplay {
namespace time {
    class GameClock;
    class GameClockTimer :
        public Timer {
    public:
        GameClockTimer(std::shared_ptr<GameClock> game_clock);
        GameClockTimer(GameClockTimer&& other) noexcept;

        GameClockTimer& operator=(GameClockTimer&& other) noexcept;

        void Cancel();

    private:
        friend GameClock;
        std::shared_ptr<GameClock> m_GameClock;

    };

}}}