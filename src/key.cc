#include "sd/framework/input/key.h"

#include <GLFW/glfw3.h>

namespace sd {
namespace framework {
namespace input {

    Key::Key() :
        id(GLFW_KEY_UNKNOWN),
        state() {}

    Key::Key(const Key& other) :
        id(other.id),
        state(other.state) {}

    Key::Key(Key&& other) noexcept :
        id(std::move(other.id)),
        state(std::move(other.state)) {}

    Key& Key::operator=(const Key& other) {
        if (this == &other) {
            return *this;
        }

        id = other.id;
        state = other.state;
        return *this;
    }

    Key& Key::operator=(Key&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        
        id = std::move(other.id);
        state = std::move(other.state);
        return *this;
    }

    bool Key::operator==(const Key& other) const noexcept {
        return id == other.id;
    }

    bool Key::operator==(int key) const noexcept {
        return id == key;
    }

    bool Key::operator!=(const Key& other) const noexcept {
        return id != other.id;
    }
}}}