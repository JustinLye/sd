#pragma once
#include <utility>
namespace sd {
namespace framework {
namespace work_items {
namespace detail {
enum class ItemStatusCode : unsigned int {
  kIncomplete,
  kComplete
};
class ItemStatus {
  ItemStatusCode m_StatusCode;
public:
  ItemStatus();
  ItemStatus(const ItemStatus& other);
  ItemStatus(ItemStatus&& other) noexcept;
  ItemStatus(ItemStatusCode status);

  ItemStatus& operator=(const ItemStatus& other);
  ItemStatus& operator=(ItemStatus&& other) noexcept;
  ItemStatus& operator=(ItemStatusCode status);
  bool operator==(const ItemStatus& other) const;
  bool operator==(ItemStatusCode other) const;

  ItemStatusCode status_code() const;
  void status_code(ItemStatusCode status);
};
inline
ItemStatus::ItemStatus() :
  m_StatusCode(ItemStatusCode::kIncomplete) {}

inline
ItemStatus::ItemStatus(const ItemStatus& other) :
  m_StatusCode(other.m_StatusCode) {}

inline
ItemStatus::ItemStatus(ItemStatus&& other) noexcept :
  m_StatusCode(std::move(other.m_StatusCode)) {}


inline
ItemStatus::ItemStatus(ItemStatusCode status) :
  m_StatusCode(status) {}

inline
ItemStatus& ItemStatus::operator=(const ItemStatus& other) {
  if (this == &other)
    return *this;
  m_StatusCode = other.m_StatusCode;
  return *this;
}

inline
ItemStatus& ItemStatus::operator=(ItemStatus&& other) noexcept {
  if (this == &other)
    return *this;
  m_StatusCode = std::move(other.m_StatusCode);
  return *this;
}

inline
ItemStatus& ItemStatus::operator=(ItemStatusCode status) {
  m_StatusCode = status;
  return *this;
}

inline
bool ItemStatus::operator==(const ItemStatus& other) const {
  return m_StatusCode == other.m_StatusCode;
}

inline
bool ItemStatus::operator==(ItemStatusCode status) const {
  return m_StatusCode == status;
}

inline
ItemStatusCode ItemStatus::status_code() const {
  return m_StatusCode;
}

inline
void ItemStatus::status_code(ItemStatusCode status) {
  m_StatusCode = status;
}

} // detail
using ItemStatusCode = sd::framework::work_items::detail::ItemStatusCode;
using ItemStatus = sd::framework::work_items::detail::ItemStatus;
} // work_items
} // framework
} // sd