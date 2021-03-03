#include "work_queue.h"

namespace sd {
namespace framework {
namespace containers {
namespace detail {
WorkQueue::WorkQueue() :
  std::queue<std::shared_ptr<framework::work_items::BaseWorkItem>>(),
  m_Mutex(),
  m_ConditionVariable(),
  m_Blocking(true) {}

WorkQueue::~WorkQueue() {}

void WorkQueue::push_work(std::shared_ptr<sd::framework::work_items::BaseWorkItem> work_item) {
  std::unique_lock<std::mutex> locker(m_Mutex);
  push(work_item);
  m_ConditionVariable.notify_all();
}

std::shared_ptr<sd::framework::work_items::BaseWorkItem> WorkQueue::get_work() {
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