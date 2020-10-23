#include "line.h"

namespace sd {
namespace geometry {

  static const glm::vec3 kStartPoint(0.0f, 1.0f, 0.0f);
  static const glm::vec3 kEndPoint(0.0f, -1.0f, 0.0f);

  static const LineData kLineData = { glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) };
  static const unsigned int kLineIndices[2] = { 0U, 1U };

  void Line::init_data() {
    m_Vertices = &kLineData;
    m_Indices = kLineIndices;
    
  }

}
}