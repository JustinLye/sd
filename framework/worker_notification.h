#pragma once
#include <type_traits>
namespace sd {
namespace framework {
namespace notifications {
namespace detail {
struct WorkerNotification { using type = WorkerNotification; size_t worker_id; };
} // detail
using WorkerNotification = detail::WorkerNotification;
} // notifications
} // framework
} // sd