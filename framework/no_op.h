#pragma once
#include <future>
#include <utility>
#include "base_work_item.h"

namespace sd {
namespace framework {
namespace work_items {
namespace detail {
class NoOp :
  public BaseWorkItem {
protected:
  void do_work_impl() override;
public:
  NoOp();
  NoOp(NoOp&& other) noexcept;
  NoOp(const NoOp&);

};

inline
void NoOp::do_work_impl() {}

inline
NoOp::NoOp() :
  BaseWorkItem() {}

inline
NoOp::NoOp(NoOp&& other) noexcept :
  BaseWorkItem(std::move(other)) {}

inline
NoOp::NoOp(const NoOp& other) :
  BaseWorkItem(other) {}


} // detail
using NoOp = detail::NoOp;
} // work_items
} // framework
} // sd