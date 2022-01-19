#pragma once

#include <ostream>
#include <string>

namespace sd {
namespace framework {
namespace input {

    enum class mouse_button_state_t : unsigned int {
        up,
        down,
        clicked,
        released,
        unknown
    };

}}}

inline std::ostream& operator<<(std::ostream& o, const sd::framework::input::mouse_button_state_t& state) {
    using st = sd::framework::input::mouse_button_state_t;
    std::string s = "";
    switch (state) {
    case st::up:
        s = "up";
        break;
    case st::down:
        s = "down";
        break;
    case st::clicked:
        s = "clicked";
        break;
    case st::released:
        s = "released";
        break;
    case st::unknown:
    default:
        s = "unknown";
        break;
    }
    o << s;
    return o;
}