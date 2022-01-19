#pragma once

#include <ostream>

namespace sd {
namespace framework {
namespace input {

    enum class mouse_action_t : unsigned int {
        press,
        release
    };

}}}

inline std::ostream& operator<<(std::ostream& o, const sd::framework::input::mouse_action_t& mouse_action) {
    std::string s = "unknown";
    switch (mouse_action) {
    case sd::framework::input::mouse_action_t::press:
        s = "press";
        break;
    case sd::framework::input::mouse_action_t::release:
        s = "release";
        break;
    }
    o << s;
    return o;
}