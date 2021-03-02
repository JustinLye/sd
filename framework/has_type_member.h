#pragma once
#include <type_traits>

namespace sd {
namespace framework {
namespace meta {
namespace detail {
template<class, class = void>
struct has_type_member : std::false_type {};
template<class T>
struct has_type_member<T, std::void_t<typename T::type>> : std::true_type {};
} // detail
template<class T>
using has_type_member = detail::has_type_member<T>;
} // meta
} // framework
} // sd