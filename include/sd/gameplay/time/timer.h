#pragma once

#include <condition_variable>
#include <future>
#include <mutex>

#include "sd/gameplay/time/timer_status_t.h"

namespace sd {
namespace gameplay {
namespace time {

    class Timer {
    public:
        Timer(std::future<timer_status_t>&& status_future);
        Timer(Timer&& other) noexcept;

        Timer& operator=(Timer&& other) noexcept;

        timer_status_t Status() const;

    private:

        mutable bool m_TimerFinished;
        mutable timer_status_t m_Status;
        mutable std::future<timer_status_t> m_StatusFuture;
    };

}}}