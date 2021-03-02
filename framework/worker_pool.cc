#if defined(_DEBUG)
#include <iostream>
#endif
#include "worker_pool.h"
#include "shutdown_worker.h"
#include "empty_work_item.h"

namespace sd {
namespace framework {
namespace detail {

void WorkerPool::initialize_workers() {
  if (m_Workers != nullptr)
    release_workers();
  std::lock_guard<std::mutex> locker(m_WorkersMutex);
  std::lock_guard<std::mutex> locker2(m_AvailableIdsMutex);
  m_Workers = new std::shared_ptr<workers::PoolWorker>[m_WorkerCount];
  for (size_t index = 0; index < m_WorkerCount; ++index) {
    m_Workers[index] = std::make_shared<workers::PoolWorker>(m_WorkQueue);
    m_Workers[index]->add(shared_from_this());
    m_Workers[index]->start();
    m_AvailableWorkerIds.push(index);
  }
}

void WorkerPool::release_workers() {
  {
    std::lock_guard<std::mutex> locker(m_WorkersMutex);
    if (m_Workers == nullptr)
      return;
    for (size_t index = 0; index < m_WorkerCount; ++index) {
      m_Workers[index]->join();
    }
    delete[]m_Workers;
    m_Workers = nullptr;
  }
  std::lock_guard<std::mutex> locker2(m_AvailableIdsMutex);
  while (!m_AvailableWorkerIds.empty()) m_AvailableWorkerIds.pop();
}

WorkerPool::WorkerPool(size_t worker_count) :
  Worker(std::shared_ptr<sd::framework::WorkQueue>(new WorkQueue())),
  m_WorkerCount(worker_count),
  m_Workers(nullptr),
  m_AvailableWorkerIds(),
  m_Mutex(),
  m_WorkerAvailable(),
  m_ShutdownRequested(false),
  m_ShutdownMutex(),
  m_ShutdownCV(),
  m_AvailableIdsMutex(),
  m_WorkersMutex() {
  m_This = std::shared_ptr<WorkerPool>(this, [](auto p) {});
  initialize_workers();
}

WorkerPool::~WorkerPool() {
  try {
    release_workers();
  }
  catch (...) {}
}

bool WorkerPool::shutdown_requested() const {
  return m_ShutdownRequested;
}


void WorkerPool::push_work(std::shared_ptr<sd::framework::work_items::BaseWorkItem> work_item) {
  if (!m_Workers)
    throw new std::runtime_error("Worker pool is not initialized.");
  {

    std::lock_guard<std::mutex> locker(m_AvailableIdsMutex);
    m_AvailableWorkerIds.pop();
  }
  m_WorkQueue->push_work(work_item);
}
void WorkerPool::shutdown() {
  if (shutdown_requested())
    return;
  m_ShutdownRequested = true;

  for (size_t index = 0; index < m_WorkerCount; ++index) {
    m_Workers[index]->stop();
  }
  m_WorkQueue->unblock();
  {
    std::unique_lock<std::mutex> locker(m_ShutdownMutex);
    while (!m_WorkQueue->empty()) {
      m_ShutdownCV.wait_for(locker, std::chrono::milliseconds(1000));
      m_WorkQueue->unblock();
    }
    
  }
  release_workers();
  
}

void WorkerPool::on_notify(std::shared_ptr<void> data) {
  on_notify(std::static_pointer_cast<notifications::WorkCompleted>(data));
}

void WorkerPool::on_notify(std::shared_ptr<notifications::WorkCompleted> data) {
  if (!data)
    return;
  {
    std::lock_guard<std::mutex> locker(m_AvailableIdsMutex);
    m_AvailableWorkerIds.push(data->worker_id);
  }
  if (shutdown_requested())
    m_ShutdownCV.notify_one();
}
} // detail
} // framework
} // sd