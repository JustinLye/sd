#pragma once

#include "glm/glm.hpp"

#include "geometry_data.h"

namespace sd {
namespace geometry {
  class Line :
    public GeometryData<glm::vec3> {
  public:
    virtual void init_data();
    virtual unsigned int vertex_count() const;
    virtual unsigned int index_count() const;
    virtual std::size_t size() const;
  };

  inline
  unsigned int Line::vertex_count() const {
    return 2U;
  }

  inline
  unsigned int Line::index_count() const {
    return 2U;
  }

  inline
  std::size_t Line::size() const {
    return sizeof(glm::vec3);
  }

}
}