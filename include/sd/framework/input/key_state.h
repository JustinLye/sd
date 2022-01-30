#pragma once

#include <chrono>
#include <ostream>

#include "sd/framework/input/key_state_type.h"

namespace sd {
namespace framework {
namespace input {
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

        inline friend std::ostream& operator<<(std::ostream& o, const KeyState& key_state);
    };

    std::ostream& operator<<(std::ostream& o, const KeyState& key_state) {
        std::string state = "";
        switch (key_state.state) {
        case key_state_t::up:
            state = "up";
            break;
        case key_state_t::down:
            state = "down";
            break;
        case key_state_t::pressed:
            state = "pressed";
            break;
        case key_state_t::released:
            state = "released";
            break;
        case key_state_t::unknown:
            state = "unknown";
            break;
        }
        o << "state: " << state << std::endl;
        return o;
    }
}}}