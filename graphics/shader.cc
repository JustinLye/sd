#include "shader.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#include <glm/gtc/type_ptr.hpp>
#include "utility.h"

namespace sd {
namespace graphics {
	Shader::Shader() : 
		m_ShaderIds(),
		m_ProgramId(0U) {}

	void Shader::load(const GLenum& shader_type, const char* source) {
		auto shader_id = OpenGLErrorWrapper(glCreateShader(shader_type));
		m_ShaderIds[shader_type] = shader_id;
		OpenGLErrorWrapper(glShaderSource(shader_id, 1, &source, NULL));
		OpenGLErrorWrapper(glCompileShader(shader_id));
		GLint shader_compiled = 0;
		OpenGLErrorWrapper(glGetShaderiv(m_ShaderIds[shader_type], GL_COMPILE_STATUS, &shader_compiled));
		if (shader_compiled == GL_FALSE) {
			GLsizei log_length = 0;
			GLchar message[1024];
			OpenGLErrorWrapper(glGetShaderInfoLog(shader_id, 1024, &log_length, message));
			throw std::runtime_error(message);
		}
		auto uniform_names = get_uniform_variable_names(source);
		auto id = program_id();
		for (auto name : uniform_names)
			m_UniformLocations.insert({ name, uniform(id, name) });
	}

	void Shader::load(const GLenum& shader_type, const std::string& source) {
		load(shader_type, source.c_str());
	}

	void Shader::load(const GLenum& shader_type, std::basic_istream<char>& source) {
		std::stringstream input_stream_content;
		input_stream_content << source.rdbuf();
		load(shader_type, input_stream_content.str());
	}

	void Shader::load(const GLenum& shader_type, const std::filesystem::path& source) {
		if (!std::filesystem::exists(source)) {
			std::cerr << "ERROR! Failed to load shader. File \'" << source << "\' does not exist." << std::endl;
			return;
		}
		std::ifstream source_file(source);
		source_file.open("r");
		load(shader_type, source_file);
		source_file.close();
	}

	GLuint Shader::program_id() const {
		if (m_ProgramId == 0)
			m_ProgramId = OpenGLErrorWrapper(glCreateProgram());
		return m_ProgramId;
	}

	void Shader::link() {
		auto id = program_id();
		std::for_each(std::begin(m_ShaderIds), std::end(m_ShaderIds),
			[this, id](auto shader_id) { OpenGLErrorWrapper(glAttachShader(id, shader_id.second)); });
		auto link_status = link_program(id);
		if (!link_status) {
			auto info_log = get_program_info_log(id);
			std::cerr << "Failed to link program: " << info_log << std::endl;
		}
		std::for_each(std::begin(m_ShaderIds), std::end(m_ShaderIds),
			[this](auto shader_id) { OpenGLErrorWrapper(glDeleteShader(shader_id.second)); });
		m_ShaderIds.clear();
	}

	void Shader::use() const {
		OpenGLErrorWrapper(glUseProgram(program_id()));
	}

	void Shader::stop_use() const {
		OpenGLErrorWrapper(glUseProgram(0));
	}

	GLint Shader::uniform(const char* name) {
		return uniform(std::string(name));
	}

	GLint Shader::uniform(const std::string& name) {
		return m_UniformLocations[name];
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

	std::vector<std::string> Shader::get_uniform_variable_names(const char* source) {
		auto source_str = std::string(source);
		return get_uniform_variable_names(source_str);
	}

	std::vector<std::string> Shader::get_uniform_variable_names(const std::string& source) {
		std::string::size_type start_pos, end_pos, curr_pos;
		std::string uniform = "uniform";
		std::string::size_type uniform_len = uniform.length();
		curr_pos = source.find(uniform, 0);
		std::vector<std::string> uniform_names;
		while (curr_pos != std::string::npos && curr_pos + uniform_len < source.length() - 1) {
			end_pos = source.find_first_of(';', curr_pos + uniform_len);
			if (end_pos != std::string::npos) {
				--end_pos;
				start_pos = source.find_last_of(' ', end_pos);
				if (start_pos != std::string::npos) {
					++start_pos;
					uniform_names.push_back(source.substr(start_pos, end_pos - start_pos + 1));
				}
			}
			curr_pos = source.find(uniform, end_pos + 1);
		}
		return uniform_names;
	}

	std::vector<std::string> Shader::get_uniform_variable_names(std::basic_istream<char>& source) {
		std::stringstream source_stream;
		source_stream << source.rdbuf();
		return get_uniform_variable_names(source_stream.str());
	}

	std::vector<std::string> Shader::get_uniform_variable_names(const std::filesystem::path& source) {
		std::ifstream source_file(source);
		source_file.open("r");
		auto variable_names = get_uniform_variable_names(source_file);
		source_file.close();
		return variable_names;
	}

	GLint Shader::uniform(GLuint program_id, const char* name) {
		return glGetUniformLocation(program_id, name);
	}

	GLint Shader::uniform(GLuint program_id, const std::string& name) {
		return uniform(program_id, name.c_str());
	}

	void Shader::UniformMat4(const char* name, const glm::mat4& value) {
		Shader::UniformMat4(program_id(), name, value);
	}

	void Shader::UniformMat4(GLuint program_id, const char* name, const glm::mat4& value) {
		glUniformMatrix4fv(uniform(program_id, name), 1, GL_FALSE, glm::value_ptr(value));
	}
}
}