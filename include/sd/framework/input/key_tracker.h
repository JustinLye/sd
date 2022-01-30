#pragma once

#include <unordered_map>

#include <GLFW/glfw3.h>

#include "sd/framework/input/key_state.h"
#include "sd/framework/interfaces/IComponent.h"
#include "sd/framework/interfaces/key_change_callback.h"

namespace sd {
namespace framework {
namespace input {

    class KeyTracker :
        public sd::framework::interfaces::IComponent {
    public:
        KeyTracker();
        
        void Initialize(GLFWwindow* window);
        void Track(int key);

        std::unordered_map<int, KeyState>::const_iterator begin() const;
        std::unordered_map<int, KeyState>::const_iterator end() const;

        const KeyState& operator[](int key) const;

        virtual void Update(double dt) override;

        std::size_t RegisterKeyChangeCallback(std::shared_ptr<framework::interfaces::IKeyChangeCallback> key_change_callback);
        void UnregisterKeyChangeCallback(std::size_t id);
    private:
        GLFWwindow* m_Window;
        std::unordered_map<int, KeyState> m_Keys;
        std::unordered_map<std::size_t, std::shared_ptr<framework::interfaces::IKeyChangeCallback>> m_KeyCallBacks;
        std::size_t m_NextKeyCallbackId;

    };

}}}