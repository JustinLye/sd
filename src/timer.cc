#include "sd/gameplay/time/timer.h"

namespace sd {
namespace gameplay {
namespace time {

    Timer::Timer(std::future<timer_status_t>&& status_future) :
        m_TimerFinished(false),
        m_Status(timer_status_t::unknown),
        m_StatusFuture(std::move(status_future)) {}

    Timer::Timer(Timer&& other) noexcept :
        m_TimerFinished(std::move(other.m_TimerFinished)),
        m_Status(std::move(other.m_Status)),
        m_StatusFuture(std::move(other.m_StatusFuture)) {}

    Timer& Timer::operator=(Timer&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        m_TimerFinished = std::move(other.m_TimerFinished);
        m_Status = std::move(other.m_Status);
        m_StatusFuture = std::move(other.m_StatusFuture);

        return *this;
    }

    timer_status_t Timer::Status() const {
        if (!m_TimerFinished && m_StatusFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            m_Status = m_StatusFuture.get();
            m_TimerFinished = true;
        }
        return m_Status;
    }

}}}