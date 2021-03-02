#pragma once
#include <memory>
#include "base_work_item.h"
#include "worker.h"

namespace sd {
namespace framework {
namespace work_items {
namespace detail {
class ShutdownWorker :
  public BaseWorkItem {
  std::shared_ptr<workers::Worker> m_Worker;
protected:
  virtual void do_work_impl() override;
public:
  ShutdownWorker(std::shared_ptr<workers::Worker> worker);
};
} // detail
using ShutdownWorker = detail::ShutdownWorker;
} // work_items
} // framework
} // sd