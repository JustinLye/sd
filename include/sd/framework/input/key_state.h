#pragma once

#include <chrono>

namespace sd {
namespace framework {
namespace input {
    enum class key_state_t : unsigned int;
    struct KeyState {
        key_state_t state;
        std::chrono::time_point<std::chrono::steady_clock> last_updated_time_point;
        KeyState();
        KeyState(const KeyState& other);
        KeyState(KeyState&& other) noexcept;

        KeyState& operator=(const KeyState& other);
        KeyState& operator=(KeyState&& other) noexcept;

        bool operator==(key_state_t state_type) const;
        bool operator!=(key_state_t state_type) const;
    };
}}}