#include "sd/framework/time/clock.h"

#include <utility>

namespace sd {
namespace framework {
namespace time {
    Clock::Clock() :
        m_HasStarted(false),
        m_IsStopped(true),
        m_StartTime(),
        m_StoppedTime(),
        m_TotalStopTime(std::chrono::nanoseconds(0)),
        m_DurationWhenStopped(std::chrono::nanoseconds(0)) {}


    Clock::Clock(const Clock& other) :
        m_HasStarted(other.m_HasStarted),
        m_IsStopped(other.m_IsStopped),
        m_StartTime(other.m_StartTime),
        m_StoppedTime(other.m_StoppedTime),
        m_TotalStopTime(other.m_TotalStopTime),
        m_DurationWhenStopped(other.m_DurationWhenStopped) {}

    Clock::Clock(Clock&& other) noexcept :
        m_HasStarted(std::move(other.m_HasStarted)),
        m_IsStopped(std::move(other.m_IsStopped)),
        m_StartTime(std::move(other.m_StartTime)),
        m_StoppedTime(std::move(other.m_StoppedTime)),
        m_TotalStopTime(std::move(other.m_TotalStopTime)),
        m_DurationWhenStopped(std::move(other.m_DurationWhenStopped)) {}

    Clock& Clock::operator=(const Clock& other) {
        if (this == &other) {
            return *this;
        }

        const std::lock_guard<std::mutex> locker(m_ClockMutex);

        m_HasStarted = other.m_HasStarted;
        m_IsStopped = other.m_IsStopped;
        m_StartTime = other.m_StartTime;
        m_StoppedTime = other.m_StoppedTime;
        m_TotalStopTime = other.m_TotalStopTime;
        m_DurationWhenStopped = other.m_DurationWhenStopped;

        return *this;
    }

    Clock& Clock::operator=(Clock&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        const std::lock_guard<std::mutex> locker(m_ClockMutex);

        m_HasStarted = std::move(other.m_HasStarted);
        m_IsStopped = std::move(other.m_IsStopped);
        m_StartTime = std::move(other.m_StartTime);
        m_StoppedTime = std::move(other.m_StoppedTime);
        m_TotalStopTime = std::move(other.m_TotalStopTime);
        m_DurationWhenStopped = std::move(other.m_DurationWhenStopped);

        return *this;
    }

    void Clock::Start() {
        const std::lock_guard<std::mutex> locker(m_ClockMutex);
        if (m_HasStarted) {
            if (m_IsStopped) {
                m_IsStopped = false;
                m_TotalStopTime += (std::chrono::high_resolution_clock::now() - m_StoppedTime);
            }
        } else {
            m_StartTime = std::chrono::high_resolution_clock::now();
            m_IsStopped = false;
            m_HasStarted = true;
        }
    }

    void Clock::Stop() {
        const std::lock_guard<std::mutex> locker(m_ClockMutex);
        if (m_HasStarted) {
            if (!m_IsStopped) {
                m_IsStopped = true;
                m_StoppedTime = std::chrono::high_resolution_clock::now();
                m_DurationWhenStopped = (m_StoppedTime - m_StartTime) - m_TotalStopTime;
            }
        }
    }

    void Clock::Reset() {
        const std::lock_guard<std::mutex> locker(m_ClockMutex);
        m_HasStarted = false;
        m_IsStopped = true;
        m_TotalStopTime = std::chrono::nanoseconds(0);
        m_DurationWhenStopped = std::chrono::nanoseconds(0);
    }

    double Clock::Elapsed() const noexcept {
        const std::lock_guard<std::mutex> locker(m_ClockMutex);
        if (!m_HasStarted) {
            return 0;
        }

        auto duration = m_IsStopped ? m_DurationWhenStopped : (std::chrono::high_resolution_clock::now() - m_StartTime) - m_TotalStopTime;
        return std::chrono::duration_cast<duration_type>(duration).count();
    }

}}}