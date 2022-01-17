#pragma once

namespace sd {
namespace gameplay {
namespace time {
    enum class timer_status_t : unsigned int {
        unknown,
        waiting,
        cancelled,
        expired
    };
}}}