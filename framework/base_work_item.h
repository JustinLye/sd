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
  std::promise<ItemStatus> m_Promise;
  ItemStatus m_ItemStatus;
protected:
  virtual void do_work_impl() = 0;
  virtual void pre_work();
  virtual void post_work();
  virtual void status(const ItemStatus& status);
  virtual void status(ItemStatusCode status);
public:
  BaseWorkItem(std::promise<ItemStatus>&& item_status);
  BaseWorkItem(BaseWorkItem&& other) noexcept;
  BaseWorkItem(const BaseWorkItem&) = delete;
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
  m_Promise.set_value(m_ItemStatus);
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
BaseWorkItem::BaseWorkItem(std::promise<ItemStatus>&& item_status) :
  m_ItemStatus(ItemStatusCode::kIncomplete),
  m_Promise(std::move(item_status)) {}

inline
BaseWorkItem::BaseWorkItem(BaseWorkItem&& other) noexcept :
  m_ItemStatus(std::move(other.m_ItemStatus)),
  m_Promise(std::move(other.m_Promise)) {}

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