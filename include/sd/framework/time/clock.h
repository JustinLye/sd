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
        virtual ~Clock();

        Clock& operator=(const Clock& other);
        Clock& operator=(Clock&& other) noexcept;

        virtual void Start();
        virtual void Stop();
        virtual void Reset();

        inline virtual bool HasStarted() const noexcept;
        inline virtual bool IsStopped() const noexcept;

        inline virtual double Elapsed() const noexcept;

        using duration_type = std::chrono::duration<double, std::chrono::seconds::period>;

    protected:
        inline virtual const std::chrono::high_resolution_clock::time_point& StartTime() const noexcept;
        inline virtual const std::chrono::high_resolution_clock::time_point& StoppedTime() const noexcept;
        inline virtual const std::chrono::high_resolution_clock::duration& TotalStopTime() const noexcept;
        inline virtual const std::chrono::high_resolution_clock::duration& DurationWhenStopped() const noexcept;

        inline virtual void HasStarted(bool has_started);
        inline virtual void IsStopped(bool is_stopped);
        inline virtual void StartTime(const std::chrono::high_resolution_clock::time_point& start_time);
        inline virtual void StoppedTime(const std::chrono::high_resolution_clock::time_point& stopped_time);
        inline virtual void TotalStopTime(const std::chrono::high_resolution_clock::duration& total_stop_time);
        inline virtual void DurationWhenStopped(const std::chrono::high_resolution_clock::duration& duration_when_stopped);

    private:
        bool m_HasStarted;
        bool m_IsStopped;
        std::chrono::high_resolution_clock::time_point m_StartTime;
        std::chrono::high_resolution_clock::time_point m_StoppedTime;
        std::chrono::high_resolution_clock::duration m_TotalStopTime;
        std::chrono::high_resolution_clock::duration m_DurationWhenStopped;
        mutable std::mutex m_ClockMutex;
    };

    bool Clock::HasStarted() const noexcept {
        return m_HasStarted;
    }

    bool Clock::IsStopped() const noexcept {
        return m_IsStopped;
    }

    const std::chrono::high_resolution_clock::time_point& Clock::StartTime() const noexcept {
        return m_StartTime;
    }
    
    const std::chrono::high_resolution_clock::time_point& Clock::StoppedTime() const noexcept {
        return m_StoppedTime;
    }
    
    const std::chrono::high_resolution_clock::duration& Clock::TotalStopTime() const noexcept {
        return m_TotalStopTime;
    }
   
    const std::chrono::high_resolution_clock::duration& Clock::DurationWhenStopped() const noexcept {
        return m_DurationWhenStopped;
    }

    void Clock::HasStarted(bool has_started) {
        std::lock_guard<std::mutex> locker(m_ClockMutex);
        m_HasStarted = has_started;
    }
    
    void Clock::IsStopped(bool is_stopped) {
        std::lock_guard<std::mutex> locker(m_ClockMutex);
        m_IsStopped = is_stopped;
    }
    
    void Clock::StartTime(const std::chrono::high_resolution_clock::time_point& start_time) {
        std::lock_guard<std::mutex> locker(m_ClockMutex);
        m_StartTime = start_time;
    }

    void Clock::StoppedTime(const std::chrono::high_resolution_clock::time_point& stopped_time) {
        std::lock_guard<std::mutex> locker(m_ClockMutex);
        m_StoppedTime = stopped_time;
    }
    
    void Clock::TotalStopTime(const std::chrono::high_resolution_clock::duration& total_stop_time) {
        std::lock_guard<std::mutex> locker(m_ClockMutex);
        m_TotalStopTime = total_stop_time;
    }
    
    void Clock::DurationWhenStopped(const std::chrono::high_resolution_clock::duration& duration_when_stopped) {
        std::lock_guard<std::mutex> locker(m_ClockMutex);
        m_DurationWhenStopped = duration_when_stopped;
    }


}}}