#include "buffer.h"
#include "vertex_array_object.h"
#include "utility.h"

namespace sd {
namespace graphics {

VertexArrayObject::VertexArrayObject(GLenum type) :
  m_VertexArrayInfo({ type, 0 }) {
  OpenGLErrorWrapperMsg(glGenVertexArrays(1, &m_VertexArrayInfo.name), "Constructing VertexArrayObject");
}

void VertexArrayObject::bind() const {
  OpenGLErrorWrapper(glBindVertexArray(m_VertexArrayInfo.name));
}

void VertexArrayObject::unbind() const {
  OpenGLErrorWrapper(glBindVertexArray(0));
}

void VertexArrayObject::add_buffer(GLuint index, const Buffer& buffer) {
  bind();
  buffer.bind();
  OpenGLErrorWrapperMsg(glVertexAttribPointer(
    index,
    buffer.component_range(),
    GL_FLOAT,
    GL_FALSE,
    static_cast<GLsizei>(buffer.component_range()) * static_cast<GLsizei>(buffer.type_size()),
    nullptr), "Adding buffer");
  OpenGLErrorWrapperMsg(glEnableVertexAttribArray(index), "enabling vertex attrib array");
}

GLenum VertexArrayObject::type() const {
  return m_VertexArrayInfo.type;
}

GLuint VertexArrayObject::name() const {
  return m_VertexArrayInfo.name;
}

}
}