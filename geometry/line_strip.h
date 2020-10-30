#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "geometry_data.h"

namespace sd {
namespace geometry {

class LineStrip :
  public GeometryData<glm::vec3> {
private:
  unsigned int m_VertexCount;
  unsigned int m_IndexCount;
protected:
  virtual void init_data();
public:
  virtual unsigned int vertex_count() const;
  virtual unsigned int index_count() const;
  virtual std::size_t size() const;
};

}
}