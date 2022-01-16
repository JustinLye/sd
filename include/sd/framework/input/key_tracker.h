#pragma once

#include <unordered_map>

#include <GLFW/glfw3.h>

#include "sd/framework/input/key.h"
#include "sd/framework/interfaces/IComponent.h"

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
    private:
        GLFWwindow* m_Window;
        std::unordered_map<int, KeyState> m_Keys;

    };

}}}