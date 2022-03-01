#pragma once

#include <unordered_map>

#include <GLFW/glfw3.h>

#include "sd/framework/input/mouse_button_state.h"
#include "sd/framework/input/mouse_button_t.h"
#include "sd/framework/interfaces/IComponent.h"
#include "sd/framework/interfaces/mouse_button_change_event_handler.h"

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

        std::size_t RegisterMouseClickCallback(std::shared_ptr<sd::framework::interfaces::IMouseButtonChangeEventHandler> mouse_click_callback);
        void UnregisterMouseClickCallback(std::size_t id);

    private:
        GLFWwindow* m_Window;
        std::unordered_map<mouse_button_t, MouseButtonState> m_Buttons;
        std::unordered_map<std::size_t, std::shared_ptr<sd::framework::interfaces::IMouseButtonChangeEventHandler>> m_MouseClickCallbacks;
        std::size_t m_NextMouseClickCallbackId;
    };

}}}