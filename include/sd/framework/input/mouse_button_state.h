#pragma once

#include <chrono>

namespace sd {
namespace framework {
namespace input {
    enum class mouse_button_state_t : unsigned int;
    struct MouseButtonState {
        mouse_button_state_t state;
        std::chrono::time_point<std::chrono::steady_clock> last_updated_time_point;

        MouseButtonState();
        MouseButtonState(const MouseButtonState& other);
        MouseButtonState(MouseButtonState&& other) noexcept;

        MouseButtonState& operator=(const MouseButtonState& other);
        MouseButtonState& operator=(MouseButtonState&& other) noexcept;

        bool operator==(mouse_button_state_t state) const noexcept;
        bool operator!=(mouse_button_state_t state) const noexcept;
    };

}}}