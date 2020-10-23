#include <iostream>
#include <fstream>
#include <filesystem>

#include "framework/program_support.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>


#include "shader.h"

void error_callback(int error, const char* description) {
	std::cerr << "ERROR! " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main(int argc, char* argv[]) {
	if (!glfwInit()) {
		std::cerr << "ERROR! Failed to initialize GLFW." << std::endl;
		return 1;
	}

	glfwSetErrorCallback(error_callback);

	auto window = glfwCreateWindow(640, 480, "Test Window", NULL, NULL);
	if (!window) {
		std::cerr << "ERROR! Failed to create window." << std::endl;
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "ERROR! Failed to load GLAD." << std::endl;
		return 1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	GLfloat verts[] = {
		-1.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	GLuint vao [2];
	glGenVertexArrays(2, vao);
	glBindVertexArray(vao[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(vao[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(9*sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	const auto sd_graphics_root = std::filesystem::path::path(sd::framework::program_support::getenv("SD_ROOT")) / "graphics";
	auto shader = sd::graphics::Shader();
	shader.load(GL_VERTEX_SHADER, sd_graphics_root / "vertex-shader.glsl");
	shader.load(GL_FRAGMENT_SHADER, sd_graphics_root / "fragment-shader.glsl");
	shader.link();

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		shader.stop_use();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}