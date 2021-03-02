#include "pool_worker.h"

namespace sd {
namespace framework {
namespace workers {
namespace detail {
size_t PoolWorker::NextId = 0;
PoolWorker::PoolWorker(std::shared_ptr<WorkQueue> work_queue) :
  Worker(work_queue),
  m_Id(++NextId),
  m_WorkCompletedNotification(std::make_shared<sd::framework::notifications::WorkCompleted>()) {
  m_WorkCompletedNotification->worker_id = m_Id;
}

void PoolWorker::do_work(std::shared_ptr<work_items::BaseWorkItem> work_item) {
  Worker::do_work(work_item);
  notify(m_WorkCompletedNotification);
}
} // detail
} // workers 
} // framework
} // sd