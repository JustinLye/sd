#pragma once

#include <condition_variable>
#include <cstdlib>
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
        virtual ~Timer();

        Timer& operator=(Timer&& other) noexcept;

        timer_status_t Status(bool wait = false) const;
        std::size_t Id() const noexcept;

    protected:
        Timer();
        void SetStatusFuture(std::future<timer_status_t>&& status_future);
    private:
        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
        std::size_t m_Id; 
        mutable bool m_TimerFinished;
        mutable timer_status_t m_Status;
        mutable std::future<timer_status_t> m_StatusFuture;
        static std::size_t s_NextId;
    };

}}}