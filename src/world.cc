#include "sd/gameplay/world.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace sd {
namespace gameplay {

    World::World() :
        sd::framework::interfaces::IComponent(),
        m_KeyTracker(),
        m_Window(nullptr),
        m_Components() {}

    bool World::Initialize() {
        if (!glfwInit()) {
            std::cerr << "Error! Failed to initialize GLFW." << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        auto window = glfwCreateWindow(600, 800, "sd", nullptr, nullptr);
        if (!window) {
            std::cerr << "Error! Failed to create window." << std::endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Error! Failed to load OpenGL." << std::endl;
            return false;
        }

        m_Components.push_back(&m_KeyTracker);

        return true;
    }

    GLFWwindow* World::Window() const {
        return m_Window;
    }

    void World::Update(double dt) {
        for (auto c : m_Components) {
            c->Update(dt);
        }
    }

}}