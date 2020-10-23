#pragma once

#include "glad/glad.h"

#include "geometry/line.h"
#include "shape.h"

namespace sd {
namespace primative_shapes {
class Line :
  Shape<geometry::Line> {
public:
  GLenum primative_type() const;
};

inline
GLenum Line::primative_type() const {
  return GL_LINE;
}
}
}