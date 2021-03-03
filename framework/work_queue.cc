#include "work_queue.h"

namespace sd {
namespace framework {
namespace containers {
namespace detail {
WorkQueue::WorkQueue() :
  std::queue<std::shared_ptr<QueuedWorkItem>>(),
  m_Mutex(),
  m_ConditionVariable(),
  m_Blocking(true) {}

WorkQueue::~WorkQueue() {}

std::future<work_items::ItemStatus> WorkQueue::push_work(std::shared_ptr<sd::framework::work_items::BaseWorkItem> work_item) {
  std::unique_lock<std::mutex> locker(m_Mutex);
  std::promise<work_items::ItemStatus> work_promise;
  auto work_future = work_promise.get_future();
  push(std::shared_ptr<QueuedWorkItem>(new QueuedWorkItem(work_item, std::move(work_promise))));
  m_ConditionVariable.notify_all();
  return work_future;
}

std::shared_ptr<QueuedWorkItem> WorkQueue::get_work() {
  std::unique_lock<std::mutex> locker(m_Mutex);
  while (empty() && m_Blocking) {
    m_ConditionVariable.wait(locker);
  }

  if (!empty()) {
    auto work_item = front();
    pop();
    return work_item;
  }
  return nullptr;

}
} // detail
} // containers
} // framework
} // sd