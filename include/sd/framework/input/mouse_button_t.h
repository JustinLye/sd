#pragma once

#include <ostream>

namespace sd {
namespace framework {
namespace input {

    enum class mouse_button_t : unsigned int {
        left,
        right,
        middle
    };

}}}

inline std::ostream& operator<<(std::ostream& o, const sd::framework::input::mouse_button_t& mouse_button) {
    std::string s = "unknown";
    using mouse_button_t = sd::framework::input::mouse_button_t;
    switch (mouse_button) {
    case mouse_button_t::left:
        s = "left";
        break;
    case mouse_button_t::middle:
        s = "middle";
        break;
    case mouse_button_t::right:
        s = "right";
        break;
    }
    o << s;
    return o;
}