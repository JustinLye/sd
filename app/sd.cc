#include <iostream>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "sd/framework/interfaces/key_change_callback.h"
#include "sd/gameplay/points.h"
#include "sd/gameplay/time/timer.h"
#include "sd/gameplay/time/timer_status_t.h"
#include "sd/gameplay/world.h"

class TestCB :
    public sd::framework::interfaces::IKeyChangeCallback {
public:
    virtual void operator()(const std::vector<std::pair<int, sd::framework::input::KeyState>>& state) override {
        for (const auto& kp : state) {
            std::cout << "Key: " << kp.first << " " << kp.second << std::endl;
        }
    }
};


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
    auto t1 = world->StartTimer(std::atoi(argv[1]));
    auto t2 = world->StartTimer(std::atoi(argv[1])/2);
    auto t = std::make_shared<TestCB>();
    world->RegisterKeyChangeCallback(t);
    std::shared_ptr<sd::gameplay::Points> points(new sd::gameplay::Points(world->Window()));
    points->Initialize();
    world->AddComponent(points);
    world->RegisterMouseClickCallback(points);
    glPointSize(1.0f);
    while (!glfwWindowShouldClose(world->Window())) {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(world->Window(), &width, &height);
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        world->Update(0);
        points->Draw();
        if (t1) {
            if (t1->Status() == sd::gameplay::time::timer_status_t::expired) {
                world->Logger() << "timer expired" << std::endl;
                t1 = nullptr;
            } else if (t1->Status() == sd::gameplay::time::timer_status_t::cancelled) {
                world->Logger() << "timer " << t1->Id() << " cancelled." << std::endl;
                t1 = nullptr;
            }
        }
        if (t2 && t2->Status() == sd::gameplay::time::timer_status_t::expired) {
            world->Logger() << "timer " << t2->Id() << " expired." << std::endl;
            world->CancelTimer(*t1);
            t2 = nullptr;
        }
        glfwSwapBuffers(world->Window());
        glfwPollEvents();
    }
    glfwDestroyWindow(world->Window());
    glfwTerminate();
    return 0;
}