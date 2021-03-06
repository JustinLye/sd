#include <cassert>
#include "worker.h"
#include "empty_work_item.h"


namespace sd {
namespace framework {
namespace workers {
namespace detail {

void Worker::work_loop() {
  while (!m_ExitWorkLoop || !m_WorkQueue->empty()) {
    auto work_item = m_WorkQueue->get_work();
    if (work_item.get() != nullptr)
    {
      assert(!work_item->complete());
      do_work(work_item);
    }
  }
}

void Worker::do_work(std::shared_ptr<work_items::BaseWorkItem> work_item) {
  work_item->do_work();
}

Worker::Worker() :
  std::thread(),
  NonCopyable(),
  m_Id(std::thread::id()),
  m_WorkQueue(),
  m_ExitWorkLoop(false) {}

Worker::Worker(std::shared_ptr<WorkQueue> work_queue) :
  std::thread(),
  NonCopyable(),
  m_Id(std::thread::id()),
  m_WorkQueue(work_queue),
  m_ExitWorkLoop(false) {}

Worker::~Worker() {}

void Worker::start() {
  if (m_Id != std::thread::id())
    throw std::runtime_error("Worker already running.");
  std::thread new_thread(std::ref(*this));
  swap(new_thread);
}

void Worker::stop() {
  m_ExitWorkLoop = true;
}

void Worker::operator()() {
  m_Id = std::this_thread::get_id();
  work_loop();
}

} // detail
} // workers
} // framework
} // sd