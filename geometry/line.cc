#include "line.h"

namespace sd {
namespace geometry {

  void Line::init_data() {
    m_Vertices[0] = glm::vec3(0.0f, 1.0f, 0.0f);
    m_Vertices[1] = glm::vec3(0.0f, -1.0f, 0.0f);
    m_Indices[0] = 0U;
    m_Indices[0] = 1U;
  }

}
}