#include "sd/framework/graphics/shaders.h"

#include <iostream>

namespace sd {
namespace framework {
namespace graphics {

    GLuint shaders::create_program(const std::vector<std::pair<GLenum, const GLchar* const>>& shaders) {
        (glGetError()); // Clear last error
        std::vector<GLuint> shader_ids;
        shader_ids.reserve(shaders.size());

        for (const auto& shader_info_pair : shaders) {
            auto shader_id = glCreateShader(shader_info_pair.first);
            if (shader_id == 0) {
                std::cerr << "ERROR! glCreateShader through exception when using GLenum " << shader_info_pair.first << ". OpenGL error code: " << glGetError() << std::endl;
                for (const auto& id : shader_ids) {
                    glDeleteShader(id);
                }
                return 0;
            }
            glShaderSource(shader_id, 1, &shader_info_pair.second, NULL);
            auto error_code = glGetError();
            if (error_code != GL_NO_ERROR) {
                std::cerr << "ERROR! glShaderSource threw exception using GLenum " << shader_info_pair.first << " and source " << shader_info_pair.second << ". OpenGL error code: " << error_code << std::endl;
                for (const auto& id : shader_ids) {
                    glDeleteShader(id);
                }
                return 0;
            }
            glCompileShader(shader_id);
            GLint success;
            glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
            if (!success) {
                GLchar info_log[512];
                glGetShaderInfoLog(shader_id, 512, NULL, info_log);
                std::cerr << "ERROR! glCompileShader failed using GLenum " << shader_info_pair.first << " and source " << shader_info_pair.second << ". Compile info: " << info_log << std::endl;
                for (const auto& id : shader_ids) {
                    glDeleteShader(id);
                }
                return 0;
            }
            shader_ids.push_back(shader_id);
        }

        auto program_id = glCreateProgram();
        for (const auto& shader_id : shader_ids) {
            glAttachShader(program_id, shader_id);
        }
        glLinkProgram(program_id);
        GLint link_success = 0;
        glGetProgramiv(program_id, GL_LINK_STATUS, &link_success);
        if (!link_success) {
            GLchar program_info_log[512];
            glGetProgramInfoLog(program_id, 512, NULL, program_info_log);
            std::cerr << "ERROR! Failed to link program. Link info: " << program_info_log << std::endl;
            for (const auto& id : shader_ids) {
                glDeleteShader(id);
            }
            glDeleteProgram(program_id);
            return 0;
        }
        for (const auto& id : shader_ids) {
            glDeleteShader(id);
        }
        return program_id;
    }

}}}