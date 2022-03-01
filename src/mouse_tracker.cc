#include "sd/framework/input/mouse_tracker.h"
#include "sd/framework/input/mouse_button_state_t.h"

namespace sd {
namespace framework {
namespace input {

    static void Update(MouseButtonState& button_state, int glfw_state);

    MouseTracker::MouseTracker() :
        interfaces::IComponent(),
        m_Window(nullptr),
        m_Buttons(),
        m_MouseClickCallbacks(),
        m_NextMouseClickCallbackId(0) {}

    void MouseTracker::Initialize(GLFWwindow* window) {
        m_Window = window;
        glfwSetInputMode(m_Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
        m_Buttons.insert({mouse_button_t::left, {}});
        m_Buttons.insert({mouse_button_t::middle, {}});
        m_Buttons.insert({mouse_button_t::right, {}});
    }

    void MouseTracker::Update(double dt) {
        auto l_button_state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT);
        auto m_button_state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_MIDDLE);
        auto r_button_state = glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT);

        auto button_iter = m_Buttons.find(mouse_button_t::left);
        auto last_state = button_iter->second;
        double xpos = 0.0;
        double ypos = 0.0;
        glfwGetCursorPos(m_Window, &xpos, &ypos);
        int height = 0;
        int width = 0;
        glfwGetFramebufferSize(m_Window, &width, &height);
        double xcoord = xpos / (static_cast<double>(width) / 2.0) - 1.0;
        double ycoord = ypos / (static_cast<double>(height) / 2.0) - 1.0;
        std::vector<MouseButtonChangeEvent> changed_button_states;
        sd::framework::input::Update(button_iter->second, l_button_state);
        if (last_state.state != button_iter->second.state) {
            changed_button_states.push_back({
                button_iter->first,
                last_state,
                button_iter->second,
                xcoord,
                ycoord});


        }
        button_iter = m_Buttons.find(mouse_button_t::middle);
        last_state = button_iter->second;
        sd::framework::input::Update(button_iter->second, m_button_state);
        if (last_state.state != button_iter->second.state) {
            changed_button_states.push_back({
                button_iter->first,
                last_state,
                button_iter->second,
                xcoord,
                ycoord });
        }
        button_iter = m_Buttons.find(mouse_button_t::right);
        last_state = button_iter->second;
        sd::framework::input::Update(button_iter->second, r_button_state);
        if (last_state.state != button_iter->second.state) {
            changed_button_states.push_back({
                button_iter->first,
                last_state,
                button_iter->second,
                xcoord,
                ycoord });
        }

        if (!changed_button_states.empty()) {
            for (auto callback : m_MouseClickCallbacks) {
                callback.second->operator()(changed_button_states);
            }
        }

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

    std::size_t MouseTracker::RegisterMouseClickCallback(std::shared_ptr<sd::framework::interfaces::IMouseButtonChangeEventHandler> mouse_click_callback) {
        auto id = ++m_NextMouseClickCallbackId;
        m_MouseClickCallbacks.insert(std::make_pair(id, mouse_click_callback));
        return id;
    }

    void MouseTracker::UnregisterMouseClickCallback(std::size_t id) {
        m_MouseClickCallbacks.erase(id);
    }


}}}