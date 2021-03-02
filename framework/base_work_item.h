#pragma once
#include <stdexcept>
namespace sd {
namespace framework {
namespace work_items {
namespace detail {

class BaseWorkItem {
private:
  bool m_Complete;
protected:
  virtual void do_work_impl() = 0;
public:
  BaseWorkItem();
  BaseWorkItem(const BaseWorkItem& other);
  virtual ~BaseWorkItem();
  void do_work();
  bool complete() const;
  virtual void reset();

};

inline
BaseWorkItem::BaseWorkItem() :
  m_Complete(false) {}

inline
BaseWorkItem::BaseWorkItem(const BaseWorkItem& other) :
  m_Complete(other.m_Complete) {}

inline
BaseWorkItem::~BaseWorkItem() {}

inline
void BaseWorkItem::do_work() {
  if (m_Complete)
    throw std::logic_error("Work is already complete");
  do_work_impl();
  m_Complete = true;
}

inline
bool BaseWorkItem::complete() const {
  return m_Complete;
}

inline
void BaseWorkItem::reset() {
  if (!m_Complete)
    throw std::logic_error("Work is not complete");
  m_Complete = false;
}

} // detail
using BaseWorkItem = detail::BaseWorkItem;
} // work_items
} // framework
} // sd