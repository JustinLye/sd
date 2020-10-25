#pragma once


#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace sd {
namespace gameplay {

struct Model {
  glm::vec3 position;
  glm::vec3 offset;
  glm::vec3 scale;
  glm::quat rotation;
};

}
}