#pragma once

#include <ostream>

#include <glm/glm.hpp>

namespace sd {
namespace framework {
namespace geometry {

    class Point {
    public:
        Point();
        Point(double value);
        Point(double x, double y, double z);
        Point(const Point& other);
        Point(Point&& other) noexcept;

        Point& operator=(const Point& other);
        Point& operator=(Point&& other) noexcept;

        const glm::vec3& Data() const;
        glm::vec3& Data();

        inline friend std::ostream& operator<<(std::ostream& out, const Point& point);
    private:
        glm::vec3 m_Point;

    };

    std::ostream& operator<<(std::ostream& out, const Point& point) {
        out << "(" << point.m_Point.x << ", " << point.m_Point.y << ", " << point.m_Point.z << ")";
        return out;
    }

}}}