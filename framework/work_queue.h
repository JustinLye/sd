#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <future>
#include <utility>
#include "base_work_item.h"

namespace sd {
namespace framework {
namespace containers {
namespace detail {

class QueuedWorkItem {
  std::shared_ptr<work_items::BaseWorkItem> m_Work;
  std::promise<work_items::ItemStatus> m_Status;
public:
  QueuedWorkItem();
  QueuedWorkItem(std::shared_ptr<work_items::BaseWorkItem> work_item, std::promise<work_items::ItemStatus>&& work_status);
  QueuedWorkItem(QueuedWorkItem&& other) noexcept;
  QueuedWorkItem(const QueuedWorkItem&) = delete;

  void work(std::shared_ptr<work_items::BaseWorkItem> work_item);
  std::shared_ptr<work_items::BaseWorkItem> work();

  void set_value(const work_items::ItemStatus& work_status);
  void set_value(work_items::ItemStatusCode work_status);
  void status(std::promise<work_items::ItemStatus>&& work_status);
};

inline
QueuedWorkItem::QueuedWorkItem() :
  m_Work(),
  m_Status() {}

inline
QueuedWorkItem::QueuedWorkItem(std::shared_ptr<work_items::BaseWorkItem> work_item, std::promise<work_items::ItemStatus>&& work_status) :
  m_Work(work_item),
  m_Status(std::move(work_status)) {}

inline
QueuedWorkItem::QueuedWorkItem(QueuedWorkItem&& other) noexcept :
  m_Work(std::move(other.m_Work)),
  m_Status(std::move(other.m_Status)) {}
/*
inline
QueuedWorkItem::QueuedWorkItem(const QueuedWorkItem& other) :
  m_Work(other.m_Work),
  m_Status(std::move(other.m_Status)) {}
  */
inline
void QueuedWorkItem::work(std::shared_ptr<work_items::BaseWorkItem> work_item) {
  m_Work = work_item;
}

inline
std::shared_ptr<work_items::BaseWorkItem> QueuedWorkItem::work() {
  return m_Work;
}

inline
void QueuedWorkItem::set_value(const work_items::ItemStatus& work_status) {
  m_Status.set_value(work_status);
}

inline
void QueuedWorkItem::set_value(work_items::ItemStatusCode work_status) {
  m_Status.set_value(work_status);
}

inline
void QueuedWorkItem::status(std::promise<work_items::ItemStatus>&& work_status) {
  m_Status = std::move(work_status);
}

class WorkQueue :
  private std::queue<std::shared_ptr<QueuedWorkItem>> {
  std::mutex m_Mutex;
  std::condition_variable m_ConditionVariable;
  bool m_Blocking;
public:
  using std::queue<std::shared_ptr<QueuedWorkItem>>::empty;
  using std::queue<std::shared_ptr<QueuedWorkItem>>::size;

  WorkQueue();
  virtual ~WorkQueue();
  virtual std::future<work_items::ItemStatus> push_work(std::shared_ptr<framework::work_items::BaseWorkItem> work_item);
  virtual std::shared_ptr<QueuedWorkItem> get_work();
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
using QueuedWorkItem = detail::QueuedWorkItem;
using WorkQueue = detail::WorkQueue;
} // containers
} // framework
} // sd