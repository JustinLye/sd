#pragma once

#include <vector>

#include "sd/framework/input/key_state.h"

namespace sd {
namespace framework {
namespace interfaces {

    class IKeyChangeCallback {
    public:
        IKeyChangeCallback() {}
        virtual ~IKeyChangeCallback() {}

        virtual void operator()(const std::vector<std::pair<int, framework::input::KeyState>>& state) = 0;
    };

}}}