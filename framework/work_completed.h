#pragma once
#include "worker_notification.h"
namespace sd {
namespace framework {
namespace notifications {
namespace detail {
struct WorkCompleted : public WorkerNotification {};
} // detail
using WorkCompleted = detail::WorkCompleted;
} // notifactions
} // framework
} // sd