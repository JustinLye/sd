#include "sd/framework/geometry/point.h"

#include <utility>

namespace sd {
namespace framework {
namespace geometry {

Point::Point() :
    m_Point(0, 0, 0) {}

Point::Point(double value) :
    m_Point(value, value, value) {}

Point::Point(double x, double y, double z) :
    m_Point(x, y, z) {}

Point::Point(const Point& other) :
    m_Point(other.m_Point) {}

Point::Point(Point&& other) noexcept :
    m_Point(std::move(other.m_Point)) {}

Point& Point::operator=(const Point& other) {
    if (this == &other) {
        return *this;
    }

    m_Point = other.m_Point;
    return *this;
}

Point& Point::operator=(Point&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    m_Point = std::move(other.m_Point);
    return *this;
}

const glm::vec3& Point::Data() const {
    return m_Point;
}

glm::vec3& Point::Data() {
    return m_Point;
}

}}}