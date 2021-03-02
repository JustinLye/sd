#include "shutdown_worker.h"

namespace sd {
namespace framework {
namespace work_items {
namespace detail {

ShutdownWorker::ShutdownWorker(std::shared_ptr<workers::Worker> worker) :
  BaseWorkItem(),
  m_Worker(worker) {}

void ShutdownWorker::do_work_impl() {
  m_Worker->stop();
}

} // detail
} // work_items
} // framework
} // sd