#include "sd/framework/input/mouse_tracker.h"
#include "sd/framework/input/mouse_button_state_t.h"

namespace sd {
namespace framework {
namespace input {

    static void Update(MouseButtonState& button_state, int glfw_state);

    MouseTracker::MouseTracker() :
        interfaces::IComponent(),
        m_Window(nullptr),
        m_Buttons() {}

    void MouseTracker::Initialize(GLFWwindow* window) {
        m_Window = window;
        glfwSetInputMode(m_Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        m_Buttons.insert({mouse_button_t::left, {}});
        m_Buttons.insert({mouse_button_t::middle, {}});
        m_Buttons.insert({mouse_button_t::right, {}});
    }

    void MouseTracker::Update(double dt) {
        MouseButtonState& mouse_button_state = m_Buttons.find(mouse_button_t::left)->second;
        auto button_state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);

        
        
    }

    void Update(MouseButtonState& button_state, int glfw_state) {
        switch (button_state.state) {
        case mouse_button_state_t::unknown:
            if (glfw_state == GLFW_PRESS) {
                button_state.state = mouse_button_state_t::clicked;
                button_state.last_updated_time_point = std::chrono::high_resolution_clock::now();
            } else if (glfw_state == GLFW_RELEASE) {
                button_state.state = mouse_button_state_t::up;
            }
            break;
        case mouse_button_state_t::clicked:
            if (glfw_state == GLFW_PRESS) {
                button_state.state = mouse_button_state_t::down;
            } else if (glfw_state == GLFW_RELEASE) {
                button_state.state = mouse_button_state_t::released;
                button_state.last_updated_time_point = std::chrono::high_resolution_clock::now();
            }
            break;
        case mouse_button_state_t::down:
            if (glfw_state == GLFW_RELEASE) {
                button_state.state = mouse_button_state_t::released;
                button_state.last_updated_time_point = std::chrono::high_resolution_clock::now();
            }
            break;
        case mouse_button_state_t::released:
            if (glfw_state == GLFW_PRESS) {
                button_state.state = mouse_button_state_t::clicked;
                button_state.last_updated_time_point = std::chrono::high_resolution_clock::now();
            } else if (glfw_state == GLFW_RELEASE) {
                button_state.state = mouse_button_state_t::up;
            }
            break;
        case mouse_button_state_t::up:
            if (glfw_state == GLFW_PRESS) {
                button_state.state = mouse_button_state_t::clicked;
                button_state.last_updated_time_point = std::chrono::high_resolution_clock::now();
            }
            break;
        }
    }

    const MouseButtonState& MouseTracker::operator[](mouse_button_t button) const noexcept {
        return m_Buttons.find(button)->second;
    }


}}}