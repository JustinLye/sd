#pragma once

#include <memory>

#include <glm/glm.hpp>

namespace sd {
namespace framework {
namespace interfaces {
    
    class IShapeData {
    public:
        IShapeData();
        virtual ~IShapeData();

        virtual unsigned int VertexCount() = 0;
        virtual unsigned int IndexCount() = 0;
    protected:
        std::shared_ptr<glm::vec3[]> m_Vertices;
        std::shared_ptr<unsigned int[]> m_Indices;
    };

}}}