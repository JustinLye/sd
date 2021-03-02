#pragma once
#include <functional>

#include "base_work_item.h"
#include "timer.h"

namespace sd {
namespace framework {
namespace work_items {
namespace detail {
class DelayedWorkItem :
  public BaseWorkItem {
  std::function<void()> m_Function;
  int m_Delay;
  Timer m_Timer;
protected:
  virtual void do_work_impl() override;
public:
  DelayedWorkItem(const std::function<void()>& function, int delay);
  DelayedWorkItem(const DelayedWorkItem& other);
  virtual ~DelayedWorkItem();
  virtual void cancel();
};

inline
DelayedWorkItem::DelayedWorkItem(const std::function<void()>& function, int delay) :
  BaseWorkItem(),
  m_Function(function),
  m_Delay(delay),
  m_Timer() {}

inline
DelayedWorkItem::DelayedWorkItem(const DelayedWorkItem& other) :
  BaseWorkItem(other),
  m_Function(other.m_Function),
  m_Delay(other.m_Delay),
  m_Timer() {}

inline
DelayedWorkItem::~DelayedWorkItem() {}

inline
void DelayedWorkItem::do_work_impl() {
  m_Timer.start(m_Function, m_Delay);
}

inline
void DelayedWorkItem::cancel() {
  m_Timer.stop();
}

} // detail
using DelayedWorkItem = detail::DelayedWorkItem;
} // work_items
} // framework
} // sd