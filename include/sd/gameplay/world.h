#pragma

#include <memory>
#include <vector>

#include "sd/framework/input/key_tracker.h"
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
    private:
        std::shared_ptr<sd::framework::input::KeyTracker> m_KeyTracker;
        GLFWwindow* m_Window;
        std::vector<std::shared_ptr<sd::framework::interfaces::IComponent>> m_Components;

        bool InitializeInput();

    };

}}