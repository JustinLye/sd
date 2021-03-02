#pragma once
#include <memory>

namespace sd {
namespace framework {
namespace detail {
class BaseObserver :
  public std::enable_shared_from_this<BaseObserver> {
  static size_t next_id;
  size_t m_Id;
public:
  BaseObserver();
  virtual ~BaseObserver();
  size_t id() const;
  virtual void on_notify(std::shared_ptr<void> data) = 0;
};
} // detail
using BaseObserver = detail::BaseObserver;
} // framework
} // sd