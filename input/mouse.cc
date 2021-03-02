#include <cassert>
#include <cmath>

#include "mouse.h"
#include "GLFW/glfw3.h"

namespace sd {
namespace input {
namespace mouse {
namespace detail {
class MouseClickHistory::HistoryByClickTime {
  MouseClickInfo* m_ClickHistory[MouseClickHistory::kMaxHistoryCapacity];

};

static bool compare_time_point_eq(const MouseClickInfo& lhs, const MouseClickInfo& rhs) {
  return lhs.click_time == rhs.click_time;
}

static bool compare_time_point_lt(const MouseClickInfo& lhs, const MouseClickInfo& rhs) {
  return lhs.click_time < rhs.click_time;
}

MouseClickInfo::MouseClickInfo() :
  x(0.0),
  y(0.0),
  click_time(high_resolution_clock::now()),
  button(kNotAMouseButton),
  is_press(false) {}

MouseClickInfo::MouseClickInfo(const MouseClickInfo& other) :
  x(other.x),
  y(other.y),
  click_time(other.click_time),
  button(other.button),
  is_press(other.is_press) {}

MouseClickInfo::MouseClickInfo(MouseClickInfo&& other) noexcept :
  x(std::move(other.x)),
  y(std::move(other.y)),
  click_time(std::move(other.click_time)),
  button(std::move(other.button)),
  is_press(std::move(other.is_press)) {}

MouseClickInfo& MouseClickInfo::operator=(const MouseClickInfo& other) {
  if (this == &other)
    return *this;
  x = other.x;
  y = other.y;
  click_time = other.click_time;
  button = other.button;
  is_press = other.is_press;
  return *this;
}

MouseClickInfo& MouseClickInfo::operator=(MouseClickInfo&& other) noexcept {
  if (this == &other)
    return *this;
  x = std::move(other.x);
  y = std::move(other.y);
  click_time = std::move(other.click_time);
  button = std::move(other.button);
  is_press = std::move(other.is_press);
  return *this;
}

bool MouseClickInfo::operator==(const MouseClickInfo& other) const {
  return x == other.x
    && y == other.y
    && click_time == other.click_time
    && button == other.button
    && is_press == other.is_press;
}

MouseClickHistory::MouseClickHistory() :
  m_Positions(),
  m_Empty(true),
  m_Full(false),
  m_CurrentIndex(0) {}

void MouseClickHistory::insert(const MouseClickInfo& mouse_click_info, size_t index) {
  // This function is here with the intention of adding additional logic/actions
  // when modifying the click history.
  m_Positions[index] = mouse_click_info;
}

void MouseClickHistory::push_back(const MouseClickInfo& mouse_click_info) {
  if (m_Empty) {
    insert(mouse_click_info, 0);
    m_Empty = false;
  }
  else {
    if (++m_CurrentIndex == kMaxHistoryCapacity) {
      m_CurrentIndex = 0;
      m_Full = true;
    }
    insert(mouse_click_info, m_CurrentIndex);
  }
}

void MouseClickHistory::clear() {
  m_Empty = true;
  m_Full = false;
  m_CurrentIndex = 0;
}

bool MouseClickHistory::empty() const {
  return m_Empty;
}

bool MouseClickHistory::full() const {
  return m_Full || (size() == kMaxHistoryCapacity);
}

std::vector<MouseClickInfo> MouseClickHistory::view(size_t count) const {
  return view(0, count);
}

std::vector<MouseClickInfo> MouseClickHistory::view(size_t start, size_t count) const {
  assert(count > 0 && "count must be greater than 0.");
  auto end = start + count;
  assert(end <= size() && "Invalid size.");
  std::vector<MouseClickInfo> output;
  for (size_t index = start; index < end; ++index)
    output.push_back(m_Positions[index]);
  return output;
}

std::vector<MouseClickInfo> MouseClickHistory::view(milliseconds milliseconds_before, const time_point<high_resolution_clock>& starting_time_point) const {
  std::vector<MouseClickInfo> output;
  /*! \todo replace this with better system */
  for (size_t index = 0; index < size(); ++index) {
    if (m_Positions[index].click_time > starting_time_point) continue;
    if (duration_cast<milliseconds>(starting_time_point - m_Positions[index].click_time) <= milliseconds_before)
      output.push_back(m_Positions[index]);
  }
  return output;
}

std::vector<MouseClickInfo> MouseClickHistory::view(milliseconds milliseconds_before, int button, const time_point<high_resolution_clock>& starting_time_point) const {
  std::vector<MouseClickInfo> output;
  for (size_t index = 0; index < size(); ++index) {
    if (m_Positions[index].click_time > starting_time_point || m_Positions[index].button == button) continue;
    if (duration_cast<milliseconds>(starting_time_point - m_Positions[index].click_time) <= milliseconds_before)
      output.push_back(m_Positions[index]);
  }
  return output;
}

size_t MouseClickHistory::size() const {
  if (m_Empty) return 0;
  if (!m_Full) return m_CurrentIndex + 1;
  return kMaxHistoryCapacity;
}

MouseInputInfo::MouseInputInfo(int button) :
  button(button),
  state(KeyState::Up),
  last_modified(high_resolution_clock::now()) {}

MouseInputInfo::MouseInputInfo(const MouseInputInfo& other) :
  button(other.button),
  state(other.state),
  last_modified(high_resolution_clock::now()) {}

MouseInputInfo::MouseInputInfo(MouseInputInfo&& other) noexcept :
  button(std::move(other.button)),
  state(std::move(other.state)),
  last_modified(std::move(other.last_modified)) {}

MouseInputInfo& MouseInputInfo::operator=(const MouseInputInfo& other) {
  if (this == &other)
    return *this;
  button = other.button;
  state = other.state;
  last_modified = other.last_modified;
  return *this;
}

MouseInputInfo& MouseInputInfo::operator=(MouseInputInfo&& other) noexcept {
  if (this == &other)
    return *this;
  button = std::move(other.button);
  state = std::move(other.state);
  last_modified = std::move(other.last_modified);
  return *this;
}

bool MouseInputInfo::operator==(const MouseInputInfo& other) const {
  return button == other.button;
}

bool MouseInputInfo::operator==(int button_name) const {
  return button == button_name;
}

bool MouseInputInfo::operator<(const MouseInputInfo& other) const {
  return button < other.button;
}

bool MouseInputInfo::operator<(int button_name) const {
  return button < button_name;
}

bool MouseInputInfo::operator>(const MouseInputInfo& other) const {
  return button > other.button;
}

bool MouseInputInfo::operator>(int button_name) const {
  return button > button_name;
}

bool MouseInputInfo::operator!=(const MouseInputInfo& other) const {
  return !(this->operator==(other));
}

bool MouseInputInfo::operator!=(int button_name) const {
  return !(this->operator==(button_name));
}

static const std::map<int, std::string> BUTTON_NAMES{
  { GLFW_MOUSE_BUTTON_LEFT, "LEFT BUTTON" },
  { GLFW_MOUSE_BUTTON_RIGHT, "RIGHT BUTTON" },
  { GLFW_MOUSE_BUTTON_MIDDLE, "MIDDLE BUTTON"}
};

static const std::map<KeyState, std::string> STATE_NAMES{
  { KeyState::Down, "DOWN" },
  { KeyState::Pressed, "PRESSED" },
  { KeyState::Up, "UP" },
  { KeyState::Released, "RELEASED" }
};

std::ostream& operator<<(std::ostream& out, const MouseInputInfo& mouse_input_info) {
  out << BUTTON_NAMES.find(mouse_input_info.button)->second << std::string(" ") << STATE_NAMES.find(mouse_input_info.state)->second << std::string(" ") << duration_cast<milliseconds>(high_resolution_clock::now() - mouse_input_info.last_modified).count() << std::string(" milliseconds");
  return out;
}

MouseInput::MouseInput() :
  m_InputStates(),
  m_InputMutex() {
  m_InputStates.insert(std::pair<int, MouseInputInfo>(GLFW_MOUSE_BUTTON_LEFT, MouseInputInfo(GLFW_MOUSE_BUTTON_LEFT)));
  m_InputStates.insert(std::pair<int, MouseInputInfo>(GLFW_MOUSE_BUTTON_RIGHT, MouseInputInfo(GLFW_MOUSE_BUTTON_RIGHT)));
  m_InputStates.insert(std::pair<int, MouseInputInfo>(GLFW_MOUSE_BUTTON_MIDDLE, MouseInputInfo(GLFW_MOUSE_BUTTON_MIDDLE)));
}

void MouseInput::update(const MouseUpdateInfo& update_info) {
  {
    std::lock_guard<std::mutex> locker(m_InputMutex);
    auto info = m_InputStates.find(update_info.button);
    assert(info != m_InputStates.end() && "Mouse button not valid.");
    if (info->second.state == update_info.state)
      return;
    info->second.state = update_info.state;
    info->second.last_modified = high_resolution_clock::now();
    if (update_info.state == KeyState::Pressed) {
      MouseClickInfo click_info;
      click_info.button = info->second.button;
      click_info.click_time = info->second.last_modified;
      click_info.is_press = true;
      click_info.x = update_info.x;
      click_info.y = update_info.y;
      m_History[info->second.button].push_back(click_info);
    }
  }
  {
    std::lock_guard<std::mutex> locker(m_HistoryMutex);

  }
}

void MouseInput::add_click(const MouseClickInfo& mouse_click_info) {
  std::lock_guard<std::mutex> locker(m_HistoryMutex);
  auto click_history = m_History.find(mouse_click_info.button);
  assert(click_history != m_History.end() && "No history found for mouse button.");
  click_history->second.push_back(mouse_click_info);
}

const MouseClickHistory& MouseInput::history(int button) const {
  std::lock_guard<std::mutex> locker(m_HistoryMutex);
  auto click_history = m_History.find(button);
  assert(click_history != m_History.end());
  return click_history->second;
}

KeyState MouseInput::state(int button) const {
  std::lock_guard<std::mutex> locker(m_InputMutex);
  auto info = m_InputStates.find(button);
  assert(info != m_InputStates.end() && "Mouse button not valid.");
  return info->second.state;
}

time_point<high_resolution_clock> MouseInput::last_modified(int button) const {
  std::lock_guard<std::mutex> locker(m_InputMutex);
  auto info = m_InputStates.find(button);
  assert(info != m_InputStates.end() && "Mouse button not valid.");
  return info->second.last_modified;
}

std::ostream& operator<<(std::ostream& out, const MouseInput& mouse_input) {
  for (const auto& state : mouse_input.m_InputStates) {
    out << state.second << std::endl;
  }
  return out;
}
} // detail
} // mouse
} // input
} // sd