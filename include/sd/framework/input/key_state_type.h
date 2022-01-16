#pragma once
namespace sd {
namespace framework {
namespace input {

    enum class key_state_t : unsigned int {
        up,
        pressed,
        released,
        down,
        unknown,
    };

}}}
