#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sd/gameplay/time/timer.h"
#include "sd/gameplay/time/timer_status_t.h"
#include "sd/gameplay/world.h"

int main(int argc, char* argv[]) {
    
#if defined(_DEBUG)
    char c;
    std::cout << "press any key to continue: ";
    std::cin >> c;
#endif
    auto world = std::make_shared<sd::gameplay::World>();
    if (!world->Initialize()) {
    } else {
    }
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(world->Window(), &width, &height);
    world->Logger() << "width: " << width << " height: " << height << std::endl;
    glViewport(0, 0, width, height);
    while (!glfwWindowShouldClose(world->Window())) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world->Update(0);
        glfwSwapBuffers(world->Window());
        glfwPollEvents();
    }
    glfwDestroyWindow(world->Window());
    glfwTerminate();
    return 0;
}