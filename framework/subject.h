#pragma once
#include <memory>
#include <map>
#include <algorithm>

#include "observer.h"
namespace sd {
namespace framework {
namespace detail {
class Subject {
protected:
  std::map <size_t, std::shared_ptr<BaseObserver>> m_Observers;
public:
  Subject();
  virtual ~Subject();
  virtual void add(std::shared_ptr<BaseObserver> observer);
  virtual void remove(std::shared_ptr<BaseObserver> observer);
  template<class T>
  void notify(std::shared_ptr<T> data) const;
};

template<class T>
void Subject::notify(std::shared_ptr<T> data) const {
  std::for_each(std::cbegin(m_Observers), std::cend(m_Observers), [&](std::pair<size_t, std::shared_ptr<BaseObserver>> iter) { iter.second->on_notify(data); });
}

} // detail
using Subject = detail::Subject;
} // framework
} // sd