#include "sd/gameplay/time/timer.h"

namespace sd {
namespace gameplay {
namespace time {
    std::size_t Timer::s_NextId = 0;

    Timer::Timer() :
        m_Id(++s_NextId),
        m_TimerFinished(false),
        m_Status(timer_status_t::unknown),
        m_StatusFuture() {}

    Timer::Timer(std::future<timer_status_t>&& status_future) :
        m_Id(++s_NextId),
        m_TimerFinished(false),
        m_Status(timer_status_t::unknown),
        m_StatusFuture(std::move(status_future)) {}

    Timer::Timer(Timer&& other) noexcept :
        m_Id(std::move(other.m_Id)),
        m_TimerFinished(std::move(other.m_TimerFinished)),
        m_Status(std::move(other.m_Status)),
        m_StatusFuture(std::move(other.m_StatusFuture)) {}

    void Timer::SetStatusFuture(std::future<timer_status_t>&& status_future) {
        if (&m_StatusFuture != &status_future) {
            m_StatusFuture = std::move(status_future);
        }
        
    }

    Timer::~Timer() {}

    Timer& Timer::operator=(Timer&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        m_Id = std::move(other.m_Id);
        m_TimerFinished = std::move(other.m_TimerFinished);
        m_Status = std::move(other.m_Status);
        m_StatusFuture = std::move(other.m_StatusFuture);

        return *this;
    }

    timer_status_t Timer::Status(bool wait) const {
        if (!m_TimerFinished) {
            if (wait) {
                m_Status = m_StatusFuture.get();
                m_TimerFinished = true;
            } else if (m_StatusFuture.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                m_Status = m_StatusFuture.get();
                m_TimerFinished = true;
            }
        }
        return m_Status;
    }

    std::size_t Timer::Id() const noexcept {
        return m_Id;
    }

}}}