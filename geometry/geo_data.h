#pragma once

#include <vector>
#include <memory>
#include <algorithm>

#include "glm/glm.hpp"

#include "framework/utility.h"

namespace sd {
namespace geometry {

using namespace framework;

struct GeoData {
  std::vector<glm::vec3> vertices;
  std::vector<unsigned int> indices;
};

class Geometry {
private:
  GeoData m_GeoData;

public:
  Geometry();
  virtual ~Geometry();

  virtual std::size_t vertex_count() const;
  virtual std::size_t index_count() const;

  virtual std::shared_ptr<glm::vec3> vertices() const;
  virtual std::shared_ptr<unsigned int> indices() const;

  virtual void vertices(const std::vector<glm::vec3>& vertices);
  virtual void vertices(const glm::vec3* vertices, std::size_t size);
  virtual void vertices(const float* vertices, std::size_t size);
  template<size_t size>
  void vertices(const glm::vec3(&vertices)[size]);
  template<size_t size>
  void vertices(const float(&vertices)[size]);
  virtual void indices(const std::vector<unsigned int>& indices);
  virtual void indices(const unsigned int* indices, std::size_t size);
  template<size_t size>
  void indices(const unsigned int(&indices)[size]);

  virtual void add_vertex(const glm::vec3& vertex);
  virtual void add_vertex(float x, float y, float z);
  virtual void add_index(unsigned int index);

};

inline
std::size_t Geometry::vertex_count() const {
  return m_GeoData.vertices.size();
}

inline
std::size_t Geometry::index_count() const {
  return m_GeoData.indices.size();
}

std::shared_ptr<glm::vec3> Geometry::vertices() const {
  if (m_GeoData.vertices.empty())
    return nullptr;
  glm::vec3* vertices_ptr = new glm::vec3[m_GeoData.vertices.size()];
  for (auto index = 0U; index < m_GeoData.vertices.size(); index++)
    vertices_ptr[index] = m_GeoData.vertices[index];
  return std::shared_ptr<glm::vec3>(vertices_ptr);
}

inline
void Geometry::vertices(const std::vector<glm::vec3>& vertices) {
  m_GeoData.vertices = vertices;
}

inline
void Geometry::vertices(const glm::vec3* vertices, std::size_t size) {
  util::containers::fill(m_GeoData.vertices, vertices, size);
}

inline
void Geometry::vertices(const float* vertices, std::size_t size) {
  assert(size % 3 == 0 && size >= 3);
  auto vertex_count = size / 3U;
  m_GeoData.vertices.clear();
  m_GeoData.vertices.resize(vertex_count);
  auto index = 0U;
  std::generate(std::begin(m_GeoData.vertices), std::end(m_GeoData.vertices),
    [vertices, &index]() {
      return glm::vec3(vertices[index++], vertices[index++], vertices[index++]);
    });
}

template<size_t size>
void Geometry::vertices(const glm::vec3(&vertices)[size]) {
  this->vertices(vertices, size);
}

template<size_t size>
void Geometry::vertices(const float(&vertices)[size]) {
  this->vertices(vertices, size);
}

inline
void Geometry::indices(const std::vector<unsigned int>& indices) {
  m_GeoData.indices = indices;
}

inline
void Geometry::indices(const unsigned int* indices, std::size_t size) {
  util::containers::fill(m_GeoData.indices, indices, size);
}

template<std::size_t size>
void Geometry::indices(const unsigned int(&indices)[size]) {
  this->indices(indices, size);
}
}
}