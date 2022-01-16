#pragma once
#include <functional>

#include "sd/framework/input/key_state.h"

namespace sd {
namespace framework {
namespace input {

    struct Key {
    public:
        Key();
        Key(const Key& other);
        Key(Key&& other) noexcept;

        Key& operator=(const Key& other);
        Key& operator=(Key&& other) noexcept;

        bool operator==(const Key& other) const noexcept;
        bool operator==(int key) const noexcept;
        bool operator!=(const Key& other) const noexcept;
        
        std::size_t id;
        KeyState state;
    };

}}}

template<>
struct std::hash<sd::framework::input::Key> {
    std::size_t operator()(const sd::framework::input::Key& key) const noexcept {
        return key.id;
    }
};