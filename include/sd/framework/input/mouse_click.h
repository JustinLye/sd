#pragma once

#include <ostream>

#include "sd/framework/input/mouse_action_t.h"
#include "sd/framework/input/mouse_button_t.h"

namespace sd {
namespace framework {
namespace input {

    struct MouseClick {
        mouse_action_t action;
        mouse_button_t button;
        double xpos;
        double ypos;
    };

}}}

inline std::ostream& operator<<(std::ostream& o, const sd::framework::input::MouseClick& mouse_click) {
    o << mouse_click.button << " " << mouse_click.action << " (" << mouse_click.xpos << ", " << mouse_click.ypos << ")";
    return o;
}