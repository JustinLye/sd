#pragma once

#include "glad/glad.h"

#include "geometry/geometry_data.h"
#include "graphics/drawable_object.h"



namespace sd {
namespace primative_shapes {

template<class GeoData>
class Shape :
  public graphics::DrawableObject {
protected:
  GeoData m_ShapeData;
  unsigned int m_VerticesVBO;
  unsigned int m_IndicesVBO;
public:
  Shape();
  virtual ~Shape();
  virtual GLenum primative_type() const = 0;
  virtual void draw() const;
  virtual void buffer();

};

template<class GeoData>
Shape<GeoData>::Shape() :
  m_ShapeData(),
  m_VerticesVBO(0U),
  m_IndicesVBO(0U) {
  m_ShapeData.init();
}

template<class GeoData>
Shape<GeoData>::~Shape() {}

template<class GeoData>
void Shape<GeoData>::draw() const {
  glDrawElements(primative_type(), m_ShapeData.index_count(), GL_UNSIGNED_INT, 0);
}

template<class GeoData>
void Shape<GeoData>::buffer() {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VerticesVBO);
  glGenBuffers(1, &m_IndicesVBO);
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VerticesVBO);
  glBufferData(GL_ARRAY_BUFFER, m_ShapeData.vertex_count() * m_ShapeData.size(), 0, GL_STATIC_DRAW);
  GLfloat* buffer = static_cast<GLfloat*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
  m_ShapeData.fill_vertices(buffer);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(m_ShapeData.size()), (void*)0);
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndicesVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_ShapeData.index_count() * sizeof(GLuint), 0, GL_STATIC_DRAW);
  GLuint* index_buffer = static_cast<GLuint*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
  m_ShapeData.fill_indices(index_buffer);
  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
  glBindVertexArray(0);
}


}
}