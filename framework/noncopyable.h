#pragma once

namespace sd {
namespace framework {
namespace detail {
class NonCopyable {
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
protected:
  NonCopyable();
  NonCopyable(NonCopyable&&) noexcept;
  NonCopyable& operator=(NonCopyable&&) noexcept;
  virtual ~NonCopyable();
};

inline
NonCopyable::NonCopyable() {}
inline
NonCopyable::NonCopyable(NonCopyable&&) noexcept {}
inline
NonCopyable& NonCopyable::operator=(NonCopyable&&) noexcept {}
inline
NonCopyable::~NonCopyable() {}
} // detail
using NonCopyable = detail::NonCopyable;
} // framework
} // sd