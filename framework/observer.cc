#include "observer.h"

namespace sd {
namespace framework {
namespace detail {
size_t BaseObserver::next_id = 0;

BaseObserver::BaseObserver() :
  std::enable_shared_from_this<BaseObserver>(),
  m_Id(++next_id) {}

BaseObserver::~BaseObserver() {}

size_t BaseObserver::id() const {
  return m_Id;
}

} // detail
} // framework
} // sd