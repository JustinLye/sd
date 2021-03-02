#include <cstring>
#include "buffer.h"
#include "utility.h"

namespace sd {
namespace graphics {


Buffer::Buffer(const BufferInfo& buffer_info, void* data) :
  m_BufferInfo(buffer_info) {
  OpenGLErrorWrapper(glGenBuffers(1, &m_BufferInfo.buffer_name));
  bind();
  OpenGLErrorWrapper(glBufferData(m_BufferInfo.binding_target,
    static_cast<GLsizeiptr>(m_BufferInfo.type_size)*static_cast<GLsizeiptr>(m_BufferInfo.component_count)*static_cast<GLsizeiptr>(m_BufferInfo.component_range),
    data,
    m_BufferInfo.usage));
}

Buffer* Buffer::Create(
  GLenum binding_target,
  int component_range,
  int component_count,
  GLfloat* data,
  GLenum usage) {
  BufferInfo buffer_info;
  buffer_info.binding_target = binding_target;
  buffer_info.type_size = sizeof(GLfloat);
  buffer_info.usage = usage;
  buffer_info.component_count = component_count;
  buffer_info.component_range = component_range;

  return new Buffer(buffer_info, (void*)data);
}

void Buffer::bind() const {
  OpenGLErrorWrapperMsg(glBindBuffer(m_BufferInfo.binding_target, m_BufferInfo.buffer_name), "binding buffer");
}

void Buffer::upload(void* data) {
  bind();
  auto buffer_ptr = OpenGLErrorWrapperMsg(glMapBuffer(m_BufferInfo.binding_target, GL_WRITE_ONLY), "getting buffer map");
  std::memcpy(buffer_ptr, data, static_cast<size_t>(m_BufferInfo.type_size)*static_cast<size_t>(m_BufferInfo.component_count)*static_cast<size_t>(m_BufferInfo.component_range));
  OpenGLErrorWrapperMsg(glUnmapBuffer(m_BufferInfo.binding_target), "unmapping buffer");
}

GLuint Buffer::type_size() const {
  return m_BufferInfo.type_size;
}

int Buffer::component_range() const {
  return m_BufferInfo.component_range;
}

int Buffer::component_count() const {
  return m_BufferInfo.component_count;
}

}
}