#pragma once

namespace sd {
namespace framework {
namespace interfaces {

    class IComponent {
    public:
        IComponent() {}
        virtual ~IComponent() {}
        virtual void Update(double dt) = 0;
    };

}}}