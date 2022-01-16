#include "sd/framework/input/key_tracker.h"

#include <chrono>

#include "sd/framework/input/key_state.h"
#include "sd/framework/input/key_state_type.h"

namespace sd {
namespace framework {
namespace input {

    KeyTracker::KeyTracker() :
        sd::framework::interfaces::IComponent(),
        m_Window(nullptr),
        m_Keys() {
        m_Keys.reserve(GLFW_KEY_LAST);
    }

    void KeyTracker::Initialize(GLFWwindow* window) {
        m_Window = window;
    }

    void KeyTracker::Track(int key) {
        m_Keys.insert({ key, KeyState() });
    }

    std::unordered_map<int, KeyState>::const_iterator KeyTracker::begin() const {
        return m_Keys.begin();
    }

    std::unordered_map<int, KeyState>::const_iterator KeyTracker::end() const {
        return m_Keys.end();
    }

    const KeyState& KeyTracker::operator[](int key) const {
        return m_Keys.find(key)->second;
    }

    void KeyTracker::Update(double dt) {
        for (auto& k : m_Keys) {
            auto key_state = glfwGetKey(m_Window, k.first);
            switch (key_state) {
            case GLFW_PRESS:
                switch (k.second.state) {
                case key_state_t::up:
                case key_state_t::unknown:
                case key_state_t::released:
                    k.second.state = key_state_t::pressed;
                    k.second.last_updated_time_point = std::chrono::steady_clock::now();
                    break;
                case key_state_t::pressed:
                    k.second.state = key_state_t::down;
                    k.second.last_updated_time_point = std::chrono::steady_clock::now();
                    break;
                }
                break;
            case GLFW_RELEASE:
                switch (k.second.state) {
                case key_state_t::down:
                case key_state_t::unknown:
                case key_state_t::pressed:
                    k.second.state = key_state_t::released;
                    k.second.last_updated_time_point = std::chrono::steady_clock::now();
                    break;
                case key_state_t::released:
                    k.second.state = key_state_t::up;
                    k.second.last_updated_time_point = std::chrono::steady_clock::now();
                    break;
                }
                break;
            case GLFW_REPEAT:
                switch (k.second.state) {
                case key_state_t::pressed:
                case key_state_t::unknown:
                case key_state_t::released:
                case key_state_t::up:
                    k.second.state = key_state_t::down;
                    k.second.last_updated_time_point = std::chrono::steady_clock::now();
                    break;

                }
                break;
            }
        }

    }


}}}