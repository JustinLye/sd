#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void error_callback(int error, const char* description) {
	std::cerr << "ERROR! " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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

	if (!gladLoadGL()) {
		std::cerr << "ERROR! Failed to load GLAD." << std::endl;
		return 1;
	}

	glfwSetKeyCallback(window, key_callback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}