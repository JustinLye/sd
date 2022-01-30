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
        m_Keys(),
        m_KeyCallBacks(), 
        m_NextKeyCallbackId(0) {
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
        std::vector<std::pair<int, KeyState>> key_changes;
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
                    key_changes.push_back(std::make_pair(k.first, k.second));
                    break;
                case key_state_t::pressed:
                    k.second.state = key_state_t::down;
                    k.second.last_updated_time_point = std::chrono::steady_clock::now();
                    key_changes.push_back(std::make_pair(k.first, k.second));
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
                    key_changes.push_back(std::make_pair(k.first, k.second));
                    break;
                case key_state_t::released:
                    k.second.state = key_state_t::up;
                    k.second.last_updated_time_point = std::chrono::steady_clock::now();
                    key_changes.push_back(std::make_pair(k.first, k.second));
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
                    key_changes.push_back(std::make_pair(k.first, k.second));
                    break;

                }
                break;
            }
        }

        for (auto& key_callback : m_KeyCallBacks) {
            key_callback.second->operator()(key_changes);
        }

    }

    std::size_t KeyTracker::RegisterKeyChangeCallback(std::shared_ptr<framework::interfaces::IKeyChangeCallback> key_change_callback) {
        auto id = ++m_NextKeyCallbackId;
        m_KeyCallBacks.insert(std::make_pair(id, key_change_callback));
        return m_NextKeyCallbackId;
    }

    void KeyTracker::UnregisterKeyChangeCallback(std::size_t id) {
        m_KeyCallBacks.erase(id);
    }

}}}