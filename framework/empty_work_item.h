#pragma once
#include "base_work_item.h"
namespace sd {
namespace framework{
namespace work_items {
namespace detail {
class EmptyWorkItem :
  public BaseWorkItem {
protected:
  virtual void do_work_impl() override {}
};
} // detail
using EmptyWorkItem = detail::EmptyWorkItem;
} // work_items
} // framework
} // sd