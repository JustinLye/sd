#pragma once
#include "glad/glad.h"

namespace sd {
namespace graphics {
class Buffer;

struct VertexArrayInfo {
  GLenum type;
  GLuint name;
};

class VertexArrayObject {
  VertexArrayInfo m_VertexArrayInfo;
public:
  VertexArrayObject(GLenum type);
  void bind() const;
  void unbind() const;
  void add_buffer(GLuint index, const Buffer& buffer);
  GLenum type() const;
  GLuint name() const;
};

}
}