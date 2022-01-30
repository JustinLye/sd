#pragma once

#include <memory>
#include <vector>

#include "sd/framework/interfaces/IComponent.h"
#include "sd/framework/interfaces/key_change_callback.h"

namespace sd {
namespace framework {
namespace input {

    class InputHub :
        public framework::interfaces::IComponent {
    public:
        InputHub();
        InputHub(const InputHub& other);
        InputHub(InputHub&& other);

        virtual void Update(double dt) override;

    private:
        std::vector<std::shared_ptr<framework::interfaces::IKeyChangeCallback>> m_KeyCallbacks;

    };

}}}