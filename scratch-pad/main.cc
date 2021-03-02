#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

#include "framework/program_support.h"
#include "framework/timer.h"
#include "graphics/shader.h"
#include "primative_shapes/line.h"
#include "geometry/line.h"
#include "graphics/buffer.h"
#include "graphics/vertex_array_object.h"
#include "graphics/utility.h"
#include "input/mouse.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void error_callback(int error_code, const char* description) {
  std::cerr << "error-code: " << error_code << "\t\'" << description << "\'" << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}



void update_input(GLFWwindow* window, sd::input::mouse::MouseInput& mouse_input) {
  using KeyState = sd::input::KeyState;
  using MouseUpdateInfo = sd::input::mouse::MouseUpdateInfo;
  double x = 0.0f;
  double y = 0.0f;
  glfwGetCursorPos(window, &x, &y);
  MouseUpdateInfo update_info;
  update_info.x = x;
  update_info.y = y;
  for (auto button : { GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE }) {
    auto mouse_state = mouse_input.state(button);
    auto glfw_state = glfwGetMouseButton(window, button);
    update_info.button = button;
    if (glfw_state == GLFW_PRESS) {
      if (mouse_state == KeyState::Up || mouse_state == KeyState::Released) {
        update_info.button = button;
        update_info.state = KeyState::Pressed;
        mouse_input.update(update_info);
      } else if (mouse_state == KeyState::Pressed) {
        update_info.state = KeyState::Down;
        mouse_input.update(update_info);
      }
    } else if (mouse_state == KeyState::Down || mouse_state == KeyState::Pressed) {
      update_info.state = KeyState::Released;
      mouse_input.update(update_info);
    } else if (mouse_state == KeyState::Released) {
      update_info.state = KeyState::Up;
      mouse_input.update(update_info);
    }
  }
}


int main(int argc, char* argv[]) {

  glfwInit();
  glfwSetErrorCallback(error_callback);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  auto window = glfwCreateWindow(800, 600, "scratch-pad", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
  }

  glViewport(0, 0, 800, 600);

  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  
  float data[] = {
    -1.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f
  };

  float data2[] = {
    -1.0f, 1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    1.0f, 1.0f, 0.0f
  };

  auto buffer = sd::graphics::Buffer::Create(
    GL_ARRAY_BUFFER,
    3,
    3,
    static_cast<GLfloat*>(data),
    GL_STATIC_DRAW);

  auto vertex_array_object = sd::graphics::VertexArrayObject(GL_TRIANGLES);
  vertex_array_object.add_buffer(0, *buffer);


  auto shader = sd::graphics::Shader();
  OpenGLErrorWrapperMsg(glBindAttribLocation(shader.program_id(), 0, "vertex_position"), "binding attrib location");
  shader.load(GL_VERTEX_SHADER, sd::framework::program_support::sd_root / "graphics" / "vertex-shader.glsl");
  shader.load(GL_FRAGMENT_SHADER, sd::framework::program_support::sd_root / "graphics" / "fragment-shader.glsl");
  shader.link();
  bool swap_buffers = false;
  void* next_data_set = (void*)data2;
  auto swap_function = [&]() { swap_buffers = true; };
  auto mouse_input = sd::input::mouse::MouseInput();
  auto print_input_function = [&]() { std::cout << mouse_input << std::endl; };
  auto timer = sd::framework::Timer();
  auto timer2 = sd::framework::Timer();
  timer.start(swap_function, 5000);
  timer2.start(print_input_function, 200);
  while (!glfwWindowShouldClose(window)) {
    update_input(window, mouse_input);
    if (swap_buffers) {
      buffer->upload(next_data_set);
      if (next_data_set == data2)
        next_data_set = (void*)data;
      else
        next_data_set = (void*)data2;
      swap_buffers = false;
      timer.start(swap_function, 5000);
    }

    if (timer2.is_expired()) {
      timer2.start(print_input_function, 200);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    vertex_array_object.bind();
    OpenGLErrorWrapperMsg(glDrawArrays(vertex_array_object.type(), 0, buffer->component_range() * buffer->component_count()), "Drawing");
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
