#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "base_work_item.h"

namespace sd {
namespace framework {
namespace containers {
namespace detail {
class WorkQueue :
  private std::queue<std::shared_ptr<framework::work_items::BaseWorkItem>> {
  std::mutex m_Mutex;
  std::condition_variable m_ConditionVariable;
  bool m_Blocking;
public:
  using std::queue<std::shared_ptr<framework::work_items::BaseWorkItem>>::empty;
  using std::queue<std::shared_ptr<framework::work_items::BaseWorkItem>>::size;

  WorkQueue();
  virtual ~WorkQueue();
  virtual void push_work(std::shared_ptr<framework::work_items::BaseWorkItem> work_item);
  virtual std::shared_ptr<framework::work_items::BaseWorkItem> get_work();
  virtual void unblock();
  virtual void block();
  virtual bool blocking() const;

};

inline
void WorkQueue::unblock() {
  m_Blocking = false;
  m_ConditionVariable.notify_all();
}

inline
void WorkQueue::block() {
  m_Blocking = true;
}

inline
bool WorkQueue::blocking() const {
  return m_Blocking;
}

} // detail
using WorkQueue = detail::WorkQueue;
} // containers
} // framework
} // sd