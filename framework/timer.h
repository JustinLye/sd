#pragma once
#include <thread>
#include <chrono>
#include <functional>
#include <mutex>

namespace sd {
namespace framework {

class Timer {
  bool m_TimerCleared;
  bool m_Expired;
  mutable std::mutex m_ClearedMutex;
public:
  Timer();
  void start(std::function<void()> f, int delay);
  void stop();
  bool is_cleared() const;
  bool is_expired() const;
};

inline
Timer::Timer() :
  m_TimerCleared(false),
  m_Expired(true),
  m_ClearedMutex() {}

inline
void Timer::start(std::function<void()> f, int delay) {
  std::lock_guard<std::mutex> locker(m_ClearedMutex);
  m_TimerCleared = false;
  m_Expired = false;
  std::thread t([=]() {
    if (this->is_cleared()) return;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    if (this->is_cleared()) return;
    f();
    m_Expired = true;
    });
  t.detach();
}

inline
void Timer::stop() {
  if (is_cleared())
    return;
  std::lock_guard<std::mutex> locker(m_ClearedMutex);
  m_TimerCleared = true;
  m_Expired = true;
}

inline
bool Timer::is_cleared() const {
  std::lock_guard<std::mutex> locker(m_ClearedMutex);
  return m_TimerCleared;
}

inline
bool Timer::is_expired() const {
  std::lock_guard<std::mutex> locker(m_ClearedMutex);
  return m_Expired;
}

}
}