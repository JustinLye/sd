#pragma once

namespace sd {
namespace framework {
namespace logging {

    enum class log_level_t : unsigned int {
        trace,
        debug,
        info,
        warning,
        critical,
        error,
        off
    };

}}}