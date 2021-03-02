#pragma once

#include <map>
#include <chrono>
#include <mutex>
#include <iostream>
#include <vector>
#include <stdexcept>

namespace sd {
namespace input {
enum class KeyState : unsigned int {
  Released,
  Up,
  Pressed,
  Down
};
namespace mouse {

namespace detail {
using namespace std::chrono;
constexpr int kNotAMouseButton = -1;

struct MouseClickInfo {
  double x;
  double y;
  time_point<high_resolution_clock> click_time;
  int button;
  bool is_press;
  MouseClickInfo();
  MouseClickInfo(const MouseClickInfo& other);
  MouseClickInfo(MouseClickInfo&& other) noexcept;
  MouseClickInfo& operator=(const MouseClickInfo& info);
  MouseClickInfo& operator=(MouseClickInfo&& info) noexcept;
  bool operator==(const MouseClickInfo& other) const;
  bool operator!() const;
  operator bool() const;
};

class MouseClickHistory {
public:
  static constexpr size_t kMaxHistoryCapacity = 1000;
private:
  class HistoryByClickTime;
  MouseClickInfo m_Positions[kMaxHistoryCapacity];
  bool m_Empty;
  bool m_Full;
  size_t m_CurrentIndex;
  void insert(const MouseClickInfo& mouse_click_info, size_t index);
public:
  MouseClickHistory();
  void push_back(const MouseClickInfo& mouse_click_info);
  void clear();
  bool empty() const;
  bool full() const;
  constexpr size_t max_capacity() const;
  std::vector<MouseClickInfo> view(size_t count) const;
  std::vector<MouseClickInfo> view(size_t start, size_t count) const;
  std::vector<MouseClickInfo> view(milliseconds milliseconds_before, const time_point<high_resolution_clock>& starting_time_point = high_resolution_clock::now()) const;
  std::vector<MouseClickInfo> view(milliseconds milliseconds_before, int button, const time_point<high_resolution_clock>& starting_time_point = high_resolution_clock::now()) const;
  template<size_t N>
  void view(MouseClickInfo(&output_array)[N]) const;
  template<size_t N>
  void view(size_t start, MouseClickInfo(&output_array)[N]) const;

  size_t size() const;

  const MouseClickInfo& operator[](size_t index) const;

};

struct MouseInputInfo {
  int button;
  KeyState state;
  time_point<high_resolution_clock> last_modified;

  MouseInputInfo(int button);
  MouseInputInfo(const MouseInputInfo& other);
  MouseInputInfo(MouseInputInfo&& other) noexcept;
  MouseInputInfo& operator=(const MouseInputInfo& other);
  MouseInputInfo& operator=(MouseInputInfo&& other) noexcept;

  bool operator==(const MouseInputInfo& other) const;
  bool operator==(int button_name) const;
  bool operator<(const MouseInputInfo& other) const;
  bool operator<(int button_name) const;
  bool operator>(const MouseInputInfo& other) const;
  bool operator>(int button_name) const;
  bool operator!=(const MouseInputInfo& other) const;
  bool operator!=(int button_name) const;

  friend std::ostream& operator<<(std::ostream& out, const MouseInputInfo& mouse_input_info);

};

struct MouseUpdateInfo {
  int button;
  KeyState state;
  double x;
  double y;
};

class MouseInput {
  std::map<int, MouseInputInfo> m_InputStates;
  mutable std::mutex m_InputMutex;
  std::map<int, MouseClickHistory> m_History;
  mutable std::mutex m_HistoryMutex;
public:
  MouseInput();
  void update(const MouseUpdateInfo& info);
  void add_click(const MouseClickInfo& mouse_click_info);
  const MouseClickHistory& history(int button) const;

  KeyState state(int button) const;
  time_point<high_resolution_clock> last_modified(int button) const;

  friend std::ostream& operator<<(std::ostream& out, const MouseInput& mouse_input);
};

inline
bool MouseClickInfo::operator!() const {
  return button == kNotAMouseButton;
}

inline
MouseClickInfo::operator bool() const {
  return !!(*this);
}

inline
const MouseClickInfo& MouseClickHistory::operator[](size_t index) const {
  if (index >= size())
    throw std::out_of_range("Index is out of range.");
  return (full() && index > m_CurrentIndex) ? m_Positions[kMaxHistoryCapacity - (index - m_CurrentIndex)] : m_Positions[m_CurrentIndex - index];
}

inline
constexpr size_t MouseClickHistory::max_capacity() const {
  return kMaxHistoryCapacity;
}

template<size_t N>
void MouseClickHistory::view(MouseClickInfo(&output_array)[N]) const {
  view(0, output_array);
}

template<size_t N>
void MouseClickHistory::view(size_t start, MouseClickInfo(&output_array)[N]) const {
  assert(N > 0 && "count must be greater than 0.");
  auto end = start + N;
  assert(end <= size() && "Invalid size.");
  for (auto index = start; index < end; ++index)
    output_array[index] = m_Positions[index];
}
} // detail
using MouseClickInfo = detail::MouseClickInfo;
using MouseClickHistory = detail::MouseClickHistory;
using MouseInputInfo = detail::MouseInputInfo;
using MouseInput = detail::MouseInput;
using MouseUpdateInfo = detail::MouseUpdateInfo;
}
}
}