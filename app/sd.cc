#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sd/gameplay/world.h"

int main(int argc, char* argv[]) {
#if defined(_DEBUG)
    char c;
    std::cout << "press any key to continue: ";
    std::cin >> c;
#endif
    auto world = std::make_shared<sd::gameplay::World>();
    if (!world->Initialize()) {
        std::cerr << "Error! World Failed to Initialize." << std::endl;
    } else {
        std::cout << "World initialized." << std::endl;
    }
    while (!glfwWindowShouldClose(world->Window())) {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(world->Window(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        world->Update(0);
        glfwSwapBuffers(world->Window());
        glfwPollEvents();
    }
    glfwDestroyWindow(world->Window());
    glfwTerminate();

    return 0;
}