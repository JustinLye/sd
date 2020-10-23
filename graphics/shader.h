#pragma once
#include <string>
#include <map>
#include <filesystem>

#include <glad/glad.h>

#include "graphics/graphics_api.h"

namespace sd {
namespace graphics {
	class SD_GRAPHICS_API Shader {
		std::map<GLenum, GLuint> m_ShaderIds;
		std::map<std::string, GLint> m_UniformLocations;
		GLuint m_ProgramId;
	public:
		Shader();

		virtual void load(const GLenum& shader_type, const char* source);
		virtual void load(const GLenum& shader_type, const std::string& source);
		virtual void load(const GLenum& shader_type, std::basic_istream<char>& source);
		virtual void load(const GLenum& shader_type, const std::filesystem::path& source);

		virtual void link();

		virtual void use() const;
		virtual void stop_use() const;

		virtual GLint uniform(const char* name);
		virtual GLint uniform(const std::string& name);

		static bool link_program(GLuint program_id);
		static bool link_status(GLuint program_id);
		static GLint get_info_log_length(GLuint program_id);
		static std::string get_program_info_log(GLuint program_id);
		static std::vector<std::string> get_uniform_variable_names(const char* source);
		static std::vector<std::string> get_uniform_variable_names(const std::string& source);
		static std::vector<std::string> get_uniform_variable_names(std::basic_istream<char>& source);
		static std::vector<std::string> get_uniform_variable_names(const std::filesystem::path& source);
		static GLint uniform(GLuint program_id, const char* name);
		static GLint uniform(GLuint program_id, const std::string& name);

	};
} // namespace graphics
} // namespace sd
