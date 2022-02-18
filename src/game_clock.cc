#include "sd/gameplay/time/game_clock.h"

#include <utility>

namespace sd {
namespace gameplay {
namespace time {

    GameClock::GameClock() :
        framework::time::Clock(),
        m_ElapsedSecondsAtLastTick(0),
        m_DeltaTime(0) {}

    GameClock::GameClock(const GameClock& other) :
        framework::time::Clock(other),
        m_ElapsedSecondsAtLastTick(other.m_ElapsedSecondsAtLastTick),
        m_DeltaTime(other.m_DeltaTime) {}

    GameClock::GameClock(GameClock&& other) noexcept :
        framework::time::Clock(other),
        m_ElapsedSecondsAtLastTick(std::move(other.m_ElapsedSecondsAtLastTick)),
        m_DeltaTime(std::move(other.m_DeltaTime)) {}

    GameClock::~GameClock() {}

    GameClock& GameClock::operator=(const GameClock& other) {
        if (this == &other) {
            return *this;
        }
        Clock::operator=(other);
        m_ElapsedSecondsAtLastTick = other.m_ElapsedSecondsAtLastTick;
        m_DeltaTime = other.m_DeltaTime;
        return *this;
    }

    GameClock& GameClock::operator=(GameClock&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        Clock::operator=(other);
        m_ElapsedSecondsAtLastTick = std::move(other.m_ElapsedSecondsAtLastTick);
        m_DeltaTime = std::move(other.m_DeltaTime);
        return *this;
    }

    double GameClock::DeltaTime() const noexcept {
        return m_DeltaTime;
    }

    bool GameClock::IsPaused() const noexcept {
        return IsStopped();
    }

    void GameClock::Tick() {
        std::unique_lock<std::mutex> locker(m_GameClockMutex);
        if (!HasStarted()) {
            Start();
        } else {
            auto elapsed = Elapsed();
            m_DeltaTime = elapsed - m_ElapsedSecondsAtLastTick;
            m_ElapsedSecondsAtLastTick = elapsed;
            m_CancelledTimerIds.clear();
        }
        m_GameClockCV.notify_all();
    }

    void GameClock::Pause() {
        Stop();
    }

    void GameClock::Resume() {
        Start();
    }

    std::shared_ptr<GameClockTimer> GameClock::StartTimer(double seconds) {
        std::shared_ptr<GameClockTimer> t(new GameClockTimer(std::shared_ptr<GameClock>(this, [](auto p) { })));
        auto f = std::async(std::launch::async, &GameClock::WaitFor, std::ref(*this), seconds, t->Id());
        t->SetStatusFuture(std::move(f));
        return t;

    }

    void GameClock::CancelTimer(const Timer& timer) {
        std::unique_lock<std::mutex> locker(m_GameClockMutex);
        m_CancelledTimerIds.insert(timer.Id());
        m_GameClockCV.notify_all();
    }

    timer_status_t GameClock::WaitFor(double seconds, std::size_t timer_id) {
        std::unique_lock<std::mutex> locker(m_GameClockMutex);
        double elapsed = 0;
        while (elapsed < seconds) {
            m_GameClockCV.wait(locker);
            elapsed += m_DeltaTime;
            if (!m_CancelledTimerIds.empty() && m_CancelledTimerIds.find(timer_id) != m_CancelledTimerIds.end()) {
                return timer_status_t::cancelled;
            }
        }
        return timer_status_t::expired;
    }
}}}