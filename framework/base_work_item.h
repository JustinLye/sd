#pragma once
#include <future>
#include <exception>

#include "item_status.h"

namespace sd {
namespace framework {
namespace work_items {
namespace detail {
class BaseWorkItem {
private:
  ItemStatus m_ItemStatus;
protected:
  virtual void do_work_impl() = 0;
  virtual void pre_work();
  virtual void post_work();
  virtual void status(const ItemStatus& status);
  virtual void status(ItemStatusCode status);
public:
  BaseWorkItem();
  BaseWorkItem(BaseWorkItem&& other) noexcept;
  BaseWorkItem(const BaseWorkItem&);
  virtual ~BaseWorkItem();
  void do_work();

  const ItemStatus& status() const;
};

inline
void BaseWorkItem::pre_work() {
  m_ItemStatus = ItemStatusCode::kIncomplete;
}

inline
void BaseWorkItem::post_work() {
  m_ItemStatus = ItemStatusCode::kComplete;
}

inline
void BaseWorkItem::status(const ItemStatus& status) {
  m_ItemStatus = status;
}

inline
void BaseWorkItem::status(ItemStatusCode status) {
  m_ItemStatus = status;
}

inline
BaseWorkItem::BaseWorkItem() :
  m_ItemStatus(ItemStatusCode::kIncomplete) {}

inline
BaseWorkItem::BaseWorkItem(BaseWorkItem&& other) noexcept :
  m_ItemStatus(std::move(other.m_ItemStatus)) {}

inline
BaseWorkItem::BaseWorkItem(const BaseWorkItem& other) :
  m_ItemStatus(other.m_ItemStatus) {}

inline
BaseWorkItem::~BaseWorkItem() {}

inline
void BaseWorkItem::do_work() {
  pre_work();
  do_work_impl();
  post_work();
}

inline
const ItemStatus& BaseWorkItem::status() const {
  return m_ItemStatus;
}

} // detail
using BaseWorkItem = sd::framework::work_items::detail::BaseWorkItem;
} // work_items
} // framework
} // sd