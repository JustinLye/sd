#pragma once

namespace sd {
namespace framework {
namespace interfaces {

    class IGameClock {
    public:
        virtual ~IGameClock() {}
        virtual double DeltaTime() const noexcept = 0;
    };

}}}
