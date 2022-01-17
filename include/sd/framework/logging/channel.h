#pragma once

#include <memory>
#include <ostream>

#include "sd/framework/logging/log_level_t.h"

namespace sd {
namespace framework {
namespace logging {

    struct Channel {
        log_level_t log_level;
        std::shared_ptr<std::ostream> stream;
    };

}}}