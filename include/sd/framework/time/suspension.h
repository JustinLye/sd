#pragma once

namespace sd {
namespace framework {
namespace time {

    enum class suspension_status_t : unsigned int {
        suspended,
        cancelled,
        expired,
        unknown
    };

}}}