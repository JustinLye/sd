#pragma once

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "sd/framework/graphics/buffer_segment.h"

namespace sd {
namespace framework {
namespace graphics {

    class ArrayBuffer {
    public:
        ArrayBuffer(GLuint vao);
        virtual ~ArrayBuffer();
        void AddPoint(const glm::vec3& point);
        void AddPoint(const std::vector<glm::vec3>& points);
        std::shared_ptr<BufferSegment> MakeSegment(GLenum poly_mode, GLuint shader_program, bool hidden = false);
        std::size_t CurrentSegmentStartPosition() const;
        std::size_t CurrentSegmentPointCount() const;
    protected:
        std::vector<glm::vec3> m_Points;
        std::vector<std::shared_ptr<BufferSegment>> m_Segments;
    };

}}}