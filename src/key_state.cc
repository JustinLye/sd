#include "sd/framework/input/key_state.h"

#include "sd/framework/input/key_state_type.h"

namespace sd {
namespace framework {
namespace input {

KeyState::KeyState() :
    state(key_state_t::unknown),
    last_updated_time_point(std::chrono::steady_clock::now()) {}

KeyState::KeyState(const KeyState& other) :
    state(other.state),
    last_updated_time_point(other.last_updated_time_point) {}

KeyState::KeyState(KeyState&& other) noexcept :
    state(std::move(other.state)),
    last_updated_time_point(std::move(other.last_updated_time_point)) {}

KeyState& KeyState::operator=(const KeyState& other) {
    if (this == &other) {
        return *this;
    }

    state = other.state;
    last_updated_time_point = other.last_updated_time_point;
    return *this;
}

KeyState& KeyState::operator=(KeyState&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    state = std::move(other.state);
    last_updated_time_point = std::move(other.last_updated_time_point);
    
    return *this;
}

bool KeyState::operator==(key_state_t state_type) const {
    return state == state_type;
}

bool KeyState::operator!=(key_state_t state_type) const {
    return state != state_type;
}

}}};