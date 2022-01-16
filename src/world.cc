#include "sd/gameplay/world.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sd/framework/input/key_state_type.h"

namespace sd {
namespace gameplay {

    World::World() :
        sd::framework::interfaces::IComponent(),
        m_KeyTracker(nullptr),
        m_Window(nullptr),
        m_Components() {}

    bool World::Initialize() {
        if (!glfwInit()) {
            std::cerr << "Error! Failed to initialize GLFW." << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        m_Window = glfwCreateWindow(600, 800, "sd", nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Error! Failed to create window." << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Error! Failed to load OpenGL." << std::endl;
            return false;
        }

        auto result = InitializeInput();

        return result;
    }

    GLFWwindow* World::Window() const {
        return m_Window;
    }

    void World::Update(double dt) {
        m_KeyTracker->Update(dt);
        using key_state_t = framework::input::key_state_t;
        switch ((*m_KeyTracker)[GLFW_KEY_ESCAPE].state) {
        case key_state_t::pressed:
        case key_state_t::down:
            glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
            break;
        }

        for (auto c : m_Components) {
            c->Update(dt);
        }
    }

    bool World::InitializeInput() {
        m_KeyTracker = std::make_shared<framework::input::KeyTracker>();
        m_KeyTracker->Initialize(m_Window);
        m_KeyTracker->Track(GLFW_KEY_ESCAPE);
        return true;
    }

}}