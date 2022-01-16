#include <functional>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char* argv[]) {
    if (!glfwInit()) {
        std::cerr << "Error! Failed to initialize GLFW." << std::endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    auto window = glfwCreateWindow(600, 800, "sd", nullptr, nullptr);
    if (!window) {
        std::cerr << "Error! Failed to create window." << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Error! Failed to load OpenGL." << std::endl;
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}