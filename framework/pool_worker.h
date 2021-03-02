#pragma once
#include <memory>

#include "worker.h"
#include "work_completed.h"
#include "subject.h"

namespace sd {
namespace framework {
namespace workers {
namespace detail {
class PoolWorker :
  public Worker,
  public Subject {
  static size_t NextId;
  size_t m_Id;
  std::shared_ptr<sd::framework::notifications::WorkCompleted> m_WorkCompletedNotification;
protected:
  virtual void do_work(std::shared_ptr<sd::framework::work_items::BaseWorkItem> work_item) override;
public:
  PoolWorker(std::shared_ptr<WorkQueue> work_queue);
};
} // detail
using PoolWorker = detail::PoolWorker;
} // workers
} // framework
} // sd