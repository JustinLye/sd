#include "sd/gameplay/time/game_clock_timer.h"

namespace sd {
namespace gameplay {
namespace time {

    GameClockTimer::GameClockTimer(std::shared_ptr<GameClock> game_clock) :
        Timer(),
        m_GameClock(game_clock) {}

    GameClockTimer::GameClockTimer(GameClockTimer&& other) noexcept :
        Timer(std::move(other)),
        m_GameClock(std::move(other.m_GameClock)) {}

    GameClockTimer& GameClockTimer::operator=(GameClockTimer&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Timer::operator=(std::move(other));
        m_GameClock = std::move(other.m_GameClock);
        return *this;
    }

    void GameClockTimer::Cancel() {
        auto status = Status();
        if (status == timer_status_t::waiting || status == timer_status_t::unknown) {
            m_GameClock->CancelTimer(*this);
        }
    }

}}}