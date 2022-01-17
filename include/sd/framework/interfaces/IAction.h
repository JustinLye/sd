#pragma once

namespace sd {
namespace framework {
namespace interfaces {

    class IAction {
    public:
        IAction() {}
        virtual ~IAction() {}

        virtual void operator()() = 0;
    };

}}}