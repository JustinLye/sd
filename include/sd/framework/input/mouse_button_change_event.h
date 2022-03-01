#pragma once

#include "sd/framework/input/mouse_button_state.h"
#include "sd/framework/input/mouse_button_state_t.h"
#include "sd/framework/input/mouse_button_t.h"

namespace sd {
namespace framework {
namespace input {

    struct MouseButtonChangeEvent {
        mouse_button_t button;
        MouseButtonState old_state;
        MouseButtonState new_state;
        double xcoord;
        double ycoord;
    };

}}}