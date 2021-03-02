#pragma once
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

#include "observer.h"
#include "worker.h"
#include "pool_worker.h"

namespace sd {
namespace framework {
namespace detail {

class WorkerPool :
  public BaseObserver,
  public workers::Worker {
  size_t m_WorkerCount;
  std::shared_ptr<workers::PoolWorker>* m_Workers;
  std::queue<size_t> m_AvailableWorkerIds;
  std::mutex m_Mutex;
  std::condition_variable m_WorkerAvailable;
  std::mutex m_ShutdownMutex;
  std::condition_variable m_ShutdownCV;
  std::shared_ptr<WorkerPool> m_This;
  std::mutex m_AvailableIdsMutex;
  std::mutex m_WorkersMutex;
  bool m_ShutdownRequested;
  void initialize_workers();
  void release_workers();
public:
  WorkerPool(size_t worker_count);
  virtual ~WorkerPool();

  virtual void on_notify(std::shared_ptr<void> data) override;
  virtual void on_notify(std::shared_ptr<notifications::WorkCompleted> data);
  
  virtual void push_work(std::shared_ptr<sd::framework::work_items::BaseWorkItem> work_item);
  virtual void shutdown();
  virtual bool shutdown_requested() const;

  size_t worker_count() const;
  bool workers_available() const;
  size_t current_workload() const;
  size_t queued_work_item_count() const;
};
inline
size_t WorkerPool::worker_count() const {
  return m_WorkerCount;
}

inline
bool WorkerPool::workers_available() const {
  return !m_AvailableWorkerIds.empty();
}

inline
size_t WorkerPool::current_workload() const {
  return worker_count() - m_AvailableWorkerIds.size();
}

inline
size_t WorkerPool::queued_work_item_count() const {
  return m_WorkQueue->size();
}

} // detail
using WorkerPool = detail::WorkerPool;
} // framework
} // sd