#pragma once
#include <glad/glad.h>
#include <memory>

namespace sd {
namespace graphics {

struct BufferInfo {
  GLuint buffer_name;
  GLenum binding_target;
  GLenum usage;
  GLuint type_size;
  int component_count;
  int component_range;
};

class Buffer {
  Buffer(const BufferInfo& buffer_info, void* data);
  Buffer() = delete;
  Buffer(const Buffer&) = delete;
  Buffer(Buffer&&) = delete;
  BufferInfo m_BufferInfo;
public:
  
  static Buffer* Create(
    GLenum binding_target,
    int component_range,
    int component_count,
    GLfloat* data = nullptr,
    GLenum usage = GL_STATIC_DRAW);

  void bind() const;
  void upload(void* data);

  GLuint type_size() const;
  int component_range() const;
  int component_count() const;

};
}
}