#pragma once

#include <unordered_map>

#include <GLFW/glfw3.h>

#include "sd/framework/input/mouse_button_state.h"
#include "sd/framework/input/mouse_button_t.h"
#include "sd/framework/interfaces/IComponent.h"

namespace sd {
namespace framework {
namespace input {


    class MouseTracker :
        public interfaces::IComponent {
    public:
        MouseTracker();

        void Initialize(GLFWwindow* window);

        virtual void Update(double dt) override;

        const MouseButtonState& operator[](mouse_button_t button) const noexcept;

    private:
        GLFWwindow* m_Window;
        std::unordered_map<mouse_button_t, MouseButtonState> m_Buttons;
    };

}}}