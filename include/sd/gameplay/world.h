#pragma

#include <memory>
#include <vector>

#include "sd/gameplay/time/game_clock.h"
#include "sd/gameplay/time/timer.h"
#include "sd/framework/input/key_tracker.h"
#include "sd/framework/logging/logger.h"
#include "sd/framework/interfaces/IComponent.h"

namespace sd {
namespace gameplay {

    class World :
        public sd::framework::interfaces::IComponent {
    public:
        World();

        bool Initialize();
        GLFWwindow* Window() const;
        virtual void Update(double dt) override;
        time::Timer StartTimer(double seconds);

    private:
        std::shared_ptr<sd::framework::input::KeyTracker> m_KeyTracker;
        GLFWwindow* m_Window;
        std::vector<std::shared_ptr<sd::framework::interfaces::IComponent>> m_Components;
        time::GameClock m_GameClock;
        framework::logging::Logger m_Logger;


        bool InitializeInput();

    };

}}