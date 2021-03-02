#pragma once

#include <iostream>
#include <string>
#include <utility>
#include "glad/glad.h"


namespace sd {
namespace graphics {
namespace utility {
inline
void OpenGLErrorEmitter(const std::string& msg = "") {
  GLenum err;
  bool msg_printed = false;
  while ((err = glGetError()) != GL_NO_ERROR) {
    if (msg != "" && !msg_printed) {
      std::cout << msg << std::endl;
      msg_printed = true; 
    }
    std::cout << "opengl error: " << err << std::endl;
  }
}
}
}
}
#if defined(_DEBUG)
#define OpenGLErrorWrapper(function_call) function_call; sd::graphics::utility::OpenGLErrorEmitter();
#define OpenGLErrorWrapperMsg(function_call, msg) function_call; sd::graphics::utility::OpenGLErrorEmitter(msg);
#else
#define OpenGLErrorWrapper(function_call)
#define OpenGLErrorWrapper(function_call, msg)
#endif