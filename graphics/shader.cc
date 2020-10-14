#include "shader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>


namespace sd {
namespace graphics {
	Shader::Shader() : 
		m_ShaderIds(),
		m_ProgramId() {}

	void Shader::load(const GLenum& shader_type, const char* source) {
		auto shader_id = glCreateShader(shader_type);
		m_ShaderIds[shader_type] = shader_id;
		glShaderSource(shader_id, 1, &source, NULL);
		glCompileShader(shader_id);
		GLint shader_compiled = 0;
		glGetShaderiv(m_ShaderIds[shader_type], GL_COMPILE_STATUS, &shader_compiled);
		if (shader_compiled == GL_FALSE) {
			GLsizei log_length = 0;
			GLchar message[1024];
			glGetShaderInfoLog(shader_id, 1024, &log_length, message);
			throw std::runtime_error(message);
		}
	}

	void Shader::load(const GLenum& shader_type, const std::string& source) {
		load(shader_type, source.c_str());
	}

	void Shader::load(const GLenum& shader_type, std::basic_istream<char>& source) {
		std::stringstream input_stream_content;
		input_stream_content << source.rdbuf();
		//auto shader_source = input_stream_content.str();
		load(shader_type, input_stream_content.str());
	}

	void Shader::load(const GLenum& shader_type, const std::filesystem::path& source) {
		std::ifstream source_file(source);
		source_file.open("r");
		load(shader_type, source_file);
		source_file.close();
	}

	void Shader::link() {
		if (m_ProgramId == 0)
			m_ProgramId = glCreateProgram();
		
		std::for_each(std::begin(m_ShaderIds), std::end(m_ShaderIds), [this](auto shader_id) { glAttachShader(m_ProgramId, shader_id.second); });
		auto link_status = link_program(m_ProgramId);
		if (!link_status) {
			auto info_log = get_program_info_log(m_ProgramId);
			std::cerr << "Failed to link program: " << info_log << std::endl;
		}
		std::for_each(std::begin(m_ShaderIds), std::end(m_ShaderIds), [this](auto shader_id) { glDeleteShader(shader_id.second); });
		m_ShaderIds.clear();
	}

	void Shader::use() const {
		glUseProgram(m_ProgramId);
	}

	void Shader::stop_use() const {
		glUseProgram(0);
	}

	bool Shader::link_program(GLuint program_id) {
		glLinkProgram(program_id);
		return link_status(program_id);
	}

	bool Shader::link_status(GLuint program_id) {
		GLint status;
		glGetProgramiv(program_id, GL_LINK_STATUS, &status);
		return status == GL_TRUE;
	}

	GLint Shader::get_info_log_length(GLuint program_id) {
		GLint info_log_length = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
		return info_log_length;
	}

	std::string Shader::get_program_info_log(GLuint program_id) {
		auto log_len = get_info_log_length(program_id);
		GLchar* info_log = new GLchar[log_len];
		glGetProgramInfoLog(program_id, log_len, NULL, info_log);
		std::string info_log_str(info_log);
		delete[] info_log;
		return info_log_str;
	}
}
}