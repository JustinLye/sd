#include "sd/gameplay/world.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sd/framework/input/key_state_type.h"
#include "sd/framework/input/mouse_button_t.h"
#include "sd/framework/input/mouse_button_state_t.h"
#include "sd/framework/input/mouse_click.h"
#include "sd/framework/logging/log_level_t.h"

namespace sd {
namespace gameplay {

    World::World() :
        sd::framework::interfaces::IComponent(),
        m_KeyTracker(nullptr),
        m_Window(nullptr),
        m_Components(),
        m_GameClock(),
        m_Logger(),
        m_MouseTracker(nullptr) {}

    bool World::Initialize() {
        m_Logger.AddChannel(std::cout, framework::logging::log_level_t::debug);
        m_Logger.AddChannel("sd.log", framework::logging::log_level_t::trace);
        if (!glfwInit()) {
            std::cerr << "Error! Failed to initialize GLFW." << std::endl;
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

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

        m_GameClock.Tick();
        m_KeyTracker->Update(m_GameClock.DeltaTime());
        m_MouseTracker->Update(m_GameClock.DeltaTime());
        using key_state_t = framework::input::key_state_t;
        using log_level_t = framework::logging::log_level_t;
        using mouse_button_t = framework::input::mouse_button_t;
        using mouse_button_state_t = framework::input::mouse_button_state_t;
        using MouseClick = framework::input::MouseClick;
        auto origin_log_level = m_Logger.LogLevel();
        switch ((*m_KeyTracker)[GLFW_KEY_ESCAPE].state) {
        case key_state_t::pressed:
        case key_state_t::down:
            glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
            break;
        }

        switch((*m_KeyTracker)[GLFW_KEY_T].state) {
        case key_state_t::pressed:
            m_Logger << log_level_t::debug << m_GameClock << std::endl << origin_log_level;
            break;
        }

        switch((*m_KeyTracker)[GLFW_KEY_F1].state) {
        case key_state_t::pressed:
            m_Logger << framework::logging::log_level_t::warning << "warning test" << std::endl << origin_log_level;
            break;
        }

        switch((*m_KeyTracker)[GLFW_KEY_P].state) {
        case key_state_t::pressed:
            if (m_GameClock.IsPaused()) {
                m_GameClock.Resume();
            } else {
                m_GameClock.Pause();
            }
            break;
        }

        if ((*m_MouseTracker)[mouse_button_t::left].state == mouse_button_state_t::clicked) {
            MouseClick mouse_click {
                framework::input::mouse_action_t::press,
                mouse_button_t::left,
                0.0,
                0.0
            };
            glfwGetCursorPos(m_Window, &mouse_click.xpos, &mouse_click.ypos);
            m_Logger << log_level_t::debug << mouse_click << std::endl;
        }

        for (auto c : m_Components) {
            c->Update(m_GameClock.DeltaTime());
        }

    }

    time::Timer World::StartTimer(double seconds) {
        return std::move(m_GameClock.StartTimer(seconds));
    }

    framework::logging::Logger& World::Logger() const {
        return m_Logger;
    }

    bool World::InitializeInput() {
        m_KeyTracker = std::make_shared<framework::input::KeyTracker>();
        m_KeyTracker->Initialize(m_Window);
        m_KeyTracker->Track(GLFW_KEY_ESCAPE);
        m_KeyTracker->Track(GLFW_KEY_T);
        m_KeyTracker->Track(GLFW_KEY_F1);
        m_KeyTracker->Track(GLFW_KEY_P);
        m_KeyTracker->Track(GLFW_KEY_KP_SUBTRACT);
        m_KeyTracker->Track(GLFW_KEY_KP_ADD);

        m_MouseTracker = std::make_shared<framework::input::MouseTracker>();
        m_MouseTracker->Initialize(m_Window);
        return true;
    }

}}