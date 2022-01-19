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
    auto timer = std::make_shared<sd::gameplay::time::Timer>(std::move(world->StartTimer(std::atoi(argv[1]))));
    auto timer1 = std::make_shared<sd::gameplay::time::Timer>(std::move(world->StartTimer(std::atoi(argv[1])*2)));
    double last_frame_time = 0;
    glfwSetTime(last_frame_time);
    while (!glfwWindowShouldClose(world->Window())) {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(world->Window(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        auto current_elapsed_time = glfwGetTime();
        world->Update(0);
        if (timer && timer->Status() == sd::gameplay::time::timer_status_t::expired) {
            std::cout << "timer expired" << std::endl;
            timer = nullptr;
        }
        if (timer1 && timer1->Status() == sd::gameplay::time::timer_status_t::expired) {
            std::cout << "timer1 expired" << std::endl;
            timer1 = nullptr;
        }
        glfwSwapBuffers(world->Window());
        glfwPollEvents();
    }
    glfwDestroyWindow(world->Window());
    glfwTerminate();
    return 0;
}