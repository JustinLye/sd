#pragma once

#include <chrono>
#include <mutex>

namespace sd {
namespace framework {
namespace time {
    class Clock {
    public:

        Clock();
        Clock(const Clock& other);
        Clock(Clock&& other) noexcept;

        Clock& operator=(const Clock& other);
        Clock& operator=(Clock&& other) noexcept;

        void Start();
        void Stop();
        void Reset();

        double Elapsed() const noexcept;

        using duration_type = std::chrono::duration<double, std::chrono::seconds::period>;

    private:
        bool m_HasStarted;
        bool m_IsStopped;
        std::chrono::high_resolution_clock::time_point m_StartTime;
        std::chrono::high_resolution_clock::time_point m_StoppedTime;
        std::chrono::high_resolution_clock::duration m_TotalStopTime;
        std::chrono::high_resolution_clock::duration m_DurationWhenStopped;
        mutable std::mutex m_ClockMutex;
    };

}}}