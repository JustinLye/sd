#pragma once

namespace sd {
namespace gameplay {

class DrawableObject {
protected:
  unsigned int m_VAO;
public:
  DrawableObject();
  virtual ~DrawableObject();
  virtual void draw() const = 0;
  virtual void buffer() = 0;
  virtual const unsigned int& vao() const;
  virtual unsigned int vao();
};

DrawableObject::DrawableObject() :
  m_VAO(0U) {}

DrawableObject::~DrawableObject() {}

inline
const unsigned int& DrawableObject::vao() const {
  return m_VAO;
}

inline
unsigned int DrawableObject::vao() {
  return m_VAO;
}
}
}