#pragma once

#include <vector>

#include <glad/glad.h>

namespace sd {
namespace framework {
namespace graphics {
    struct shaders {
        struct vertex {
            static constexpr const GLchar* const default_shader = R"(
#version 330 core
layout (location=0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";
        };
        struct fragment {
            static constexpr const GLchar* const default_shader = R"(
#version 330 core
out vec4 fragColor;
void main() {
    fragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
)";
            static constexpr const GLchar* const red_pixel = R"(
#version 330 core
out vec4 fragColor;
void main() {
    fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
)";
        };

        static GLuint create_program(const std::vector<std::pair<GLenum, const GLchar* const>>& shaders);
    };
}}}