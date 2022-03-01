#pragma once

#include <vector>

#include "sd/framework/input/mouse_button_state.h"
#include "sd/framework/input/mouse_button_t.h"
#include "sd/framework/input/mouse_button_change_event.h"

namespace sd {
namespace framework {
namespace interfaces {

    class IMouseButtonChangeEventHandler {
    public:
        IMouseButtonChangeEventHandler() {}
        virtual ~IMouseButtonChangeEventHandler() {}

        virtual void operator()(const std::vector<sd::framework::input::MouseButtonChangeEvent>& events) = 0;
    };

}}}