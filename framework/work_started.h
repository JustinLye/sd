#pragma once
#include "worker_notification.h"
namespace sd {
namespace framework {
namespace notifications {
namespace detail {
struct WorkStarted : public WorkerNotification {};
} // detail
using WorkStarted = detail::WorkStarted;
} // notifications
} // framework
} // sd