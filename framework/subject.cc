#include "subject.h"
#include <algorithm>

namespace sd {
namespace framework {
namespace detail {
Subject::Subject() {}

Subject::~Subject() {}

void Subject::add(std::shared_ptr<BaseObserver> observer) {
  if (m_Observers.find(observer->id()) == m_Observers.cend())
    m_Observers.insert({ observer->id(), observer });
}

void Subject::remove(std::shared_ptr<BaseObserver> observer) {
  m_Observers.erase(observer->id());
}

} // detail
} // framework
} // sd