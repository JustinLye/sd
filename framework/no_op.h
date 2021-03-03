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
  NoOp(std::promise<ItemStatus>&& item_status);
  NoOp(NoOp&& other) noexcept;
  NoOp(const NoOp&) = delete;

};

inline
void NoOp::do_work_impl() {}

inline
NoOp::NoOp(std::promise<ItemStatus>&& item_status) :
  BaseWorkItem(std::move(item_status)) {}

inline
NoOp::NoOp(NoOp&& other) noexcept :
  BaseWorkItem(std::move(other)) {}


} // detail
using NoOp = detail::NoOp;
} // work_items
} // framework
} // sd