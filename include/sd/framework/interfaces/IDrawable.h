#pragma once

namespace sd {
namespace framework {
namespace interfaces {

    class IDrawable {
    public:
        IDrawable() {}
        virtual ~IDrawable() {}
        virtual void Draw() = 0;
    };

}}}