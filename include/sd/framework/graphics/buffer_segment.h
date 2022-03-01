#pragma once

#include <cstdlib>

#include <glad/glad.h>

namespace sd {
namespace framework {
namespace graphics {

    struct BufferSegment {
        GLenum poly_mode;
        std::size_t offset;
        std::size_t size;
        bool hidden;
        GLuint shader_program;
    };

}}}