#pragma once
#include <type_traits>
#include "has_type_member.h"
#include "worker_notification.h"

namespace sd {
namespace framework {
namespace meta {
namespace detail {
template<class, class = void>
struct is_worker_notification : std::false_type {};

template<class T>
struct is_worker_notification<
  T,
  std::void_t<typename std::enable_if_t<meta::has_type_member<T>::value>>> {
  static constexpr bool value = std::is_same_v<T::type, sd::framework::notifications::WorkerNotification>;
};

} // detail
template<class T>
using is_worker_notification = detail::is_worker_notification<T>;
} // meta
} // framework
} // sd