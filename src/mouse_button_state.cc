#include "sd/framework/input/mouse_button_state.h"

#include <utility>

#include "sd/framework/input/mouse_button_state_t.h"

namespace sd {
namespace framework {
namespace input {

    MouseButtonState::MouseButtonState() :
        state(mouse_button_state_t::unknown),
        last_updated_time_point(std::chrono::high_resolution_clock::now()) {}

    MouseButtonState::MouseButtonState(const MouseButtonState& other) :
        state(other.state),
        last_updated_time_point(other.last_updated_time_point) {}

    MouseButtonState::MouseButtonState(MouseButtonState&& other) noexcept :
        state(std::move(other.state)),
        last_updated_time_point(std::move(other.last_updated_time_point)) {}

    MouseButtonState& MouseButtonState::operator=(const MouseButtonState& other) {
        if (this == &other) {
            return *this;
        }

        state = other.state;
        last_updated_time_point = other.last_updated_time_point;

        return *this;
    }

    MouseButtonState& MouseButtonState::operator=(MouseButtonState&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        state = std::move(other.state);
        last_updated_time_point = std::move(other.last_updated_time_point);

        return *this;
    }

    bool MouseButtonState::operator==(mouse_button_state_t state) const noexcept {
        return this->state == state;
    }

    bool MouseButtonState::operator!=(mouse_button_state_t state) const noexcept {
        return !(*this == state);
    }

}}}