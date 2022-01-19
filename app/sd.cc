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
    auto t1 = std::shared_ptr<sd::gameplay::time::Timer>(new sd::gameplay::time::Timer(std::move(world->StartTimer(std::atoi(argv[1])))));
    while (!glfwWindowShouldClose(world->Window())) {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(world->Window(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world->Update(0);
        if (t1 && t1->Status() == sd::gameplay::time::timer_status_t::expired) {
            world->Logger() << "timer expired" << std::endl;
            t1 = nullptr;
        }
        glfwSwapBuffers(world->Window());
        glfwPollEvents();
    }
    glfwDestroyWindow(world->Window());
    glfwTerminate();
    return 0;
}