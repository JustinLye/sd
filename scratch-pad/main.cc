#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

#include "framework/program_support.h"
#include "graphics/shader.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"




void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char* argv[]) {

  glfwInit();
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

  auto vbo = 0U;
  auto vao = 0U;
  glGenBuffers(1, &vbo);
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  float vertices[] = {
    0.0f, -1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  auto shader = sd::graphics::Shader();
  shader.load(GL_VERTEX_SHADER, sd::framework::program_support::sd_root / "graphics" / "vertex-shader.glsl");
  shader.load(GL_FRAGMENT_SHADER, sd::framework::program_support::sd_root / "graphics" / "fragment-shader.glsl");
  shader.link();

  while (!glfwWindowShouldClose(window)) {
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 2);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
