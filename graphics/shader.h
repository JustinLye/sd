#pragma once
#include <string>
#include <map>
#include <filesystem>

#include <glad/glad.h>

namespace sd {
namespace graphics {
	class Shader {
		std::map<GLenum, GLuint> m_ShaderIds;
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

		static bool link_program(GLuint program_id);
		static bool link_status(GLuint program_id);
		static GLint get_info_log_length(GLuint program_id);
		static std::string get_program_info_log(GLuint program_id);

	};
} // namespace graphics
} // namespace sd
