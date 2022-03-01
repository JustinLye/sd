#pragma once

#include <memory>
#include <vector>

#include "sd/gameplay/time/game_clock.h"
#include "sd/gameplay/time/timer.h"
#include "sd/framework/input/key_tracker.h"
#include "sd/framework/input/mouse_tracker.h"
#include "sd/framework/logging/logger.h"
#include "sd/framework/interfaces/IComponent.h"
#include "sd/framework/interfaces/key_change_callback.h"
#include "sd/framework/interfaces/mouse_button_change_event_handler.h"

namespace sd {
namespace gameplay {

    class World :
        public sd::framework::interfaces::IComponent {
    public:
        World();

        bool Initialize();
        GLFWwindow* Window() const;
        virtual void Update(double dt) override;
        std::shared_ptr<time::GameClockTimer> StartTimer(double seconds);
        void CancelTimer(const time::Timer& timer);
        framework::logging::Logger& Logger() const;

        std::size_t RegisterKeyChangeCallback(std::shared_ptr<framework::interfaces::IKeyChangeCallback> key_change_callback);
        void UnregisterKeyChangeCallback(std::size_t id);
        std::size_t RegisterMouseClickCallback(std::shared_ptr<framework::interfaces::IMouseButtonChangeEventHandler> mouse_click_callback);
        void UnregisterMouseClickCallback(std::size_t id);
        void AddComponent(std::shared_ptr<sd::framework::interfaces::IComponent> component);

        void Pause();
        void Resume();

    private:
        std::shared_ptr<sd::framework::input::KeyTracker> m_KeyTracker;
        GLFWwindow* m_Window;
        std::vector<std::shared_ptr<sd::framework::interfaces::IComponent>> m_Components;
        time::GameClock m_GameClock;
        mutable framework::logging::Logger m_Logger;
        std::shared_ptr<sd::framework::input::MouseTracker> m_MouseTracker;
        std::pair<double, double> m_CurrentScreenCoords;

        bool InitializeInput();

    };

}}