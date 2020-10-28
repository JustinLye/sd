#pragma once
#include <cstdlib>
namespace sd {
namespace geometry {
  using vertex_t = float;
  using index_t = unsigned int;

  template<class DataType>
  class GeometryData {
  protected:
    DataType* m_Vertices;
    unsigned int* m_Indices;
    
    virtual void init_pointers();
    virtual void init_data() = 0;
    virtual void destroy();
  public:
    GeometryData();
    virtual ~GeometryData();

    virtual void init();
    virtual unsigned int vertex_count() const = 0;
    virtual unsigned int index_count() const = 0; 
    virtual std::size_t size() const = 0;

    virtual void fill_vertices(vertex_t* data_buffer);
    virtual void fill_indices(index_t* data_buffer);
  };

  template<class DataType>
  GeometryData<DataType>::GeometryData() :
    m_Vertices(nullptr),
    m_Indices(nullptr) {
  }

  template<class DataType>
  void GeometryData<DataType>::init() {
    init_pointers();
    init_data();
  }

  template<class DataType>
  void GeometryData<DataType>::init_pointers() {
    destroy();
    m_Vertices = new DataType[vertex_count()];
    m_Indices = new unsigned int[index_count()];
  }

  template<class DataType>
  void GeometryData<DataType>::destroy() {
    if (m_Vertices != nullptr)
      delete[] m_Vertices;
    if (m_Indices != nullptr)
      delete[] m_Indices;
    m_Vertices = nullptr;
    m_Indices = nullptr;
  }

  template<class DataType>
  GeometryData<DataType>::~GeometryData() {
    try {
      destroy();
    } catch (...) {

    }
  }

  template<class DataType>
  void GeometryData<DataType>::fill_vertices(vertex_t* data_buffer) {
    DataType* data = (DataType*)data_buffer;
    for (auto index = 0U; index < vertex_count(); ++index)
      data[index] = m_Vertices[index];
  }

  template<class DataType>
  void GeometryData<DataType>::fill_indices(index_t* data_buffer) {
    index_t* indices = data_buffer;
    for (auto index = 0U; index < index_count(); ++index)
      data_buffer[index] = m_Indices[index];
  }
}
}
