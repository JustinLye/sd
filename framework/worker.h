#pragma once
#include <thread>
#include <memory>
#include "noncopyable.h"
#include "work_queue.h"
#include "base_work_item.h"

namespace sd {
namespace framework {
namespace workers {
namespace detail {
class Worker :
  private std::thread,
  public NonCopyable {
protected:
  std::thread::id m_Id;
  std::shared_ptr<WorkQueue> m_WorkQueue;
  bool m_ExitWorkLoop;
  virtual void work_loop();
  Worker();
  virtual void do_work(std::shared_ptr<work_items::BaseWorkItem> work_item);
public:
  Worker(std::shared_ptr<WorkQueue> work_queue);
  virtual ~Worker();
  using std::thread::get_id;
  using std::thread::join;
  using std::thread::joinable;
  virtual void start();
  virtual void stop();
  virtual void operator()();
};
} // detail
using Worker = detail::Worker;
} // workers
} // framework
} // sd