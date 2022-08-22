#pragma once

#include <memory>

#include "sd/framework/geometry/mesh.h"

namespace sd {
namespace framework {
namespace scene {

    class Node {
    public:
        virtual ~Node() {}

        virtual void Update(double dt) = 0;
        virtual const std::shared_ptr<geometry::Mesh>& Mesh() const;
        virtual void Mesh(std::shared_ptr<geometry::Mesh> mesh);

    };

}}}