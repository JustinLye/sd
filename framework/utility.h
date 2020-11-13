#pragma once
#include <vector>
#include <algorithm>

namespace sd {
namespace framework {
namespace util {
namespace containers {
  template<class T>
  void fill(std::vector<T>& out_vec, const T* input_array, std::size_t count) {
    out_vec.clear();
    out_vec.resize(count);
    auto index = 0U;
    std::generate_n(
      std::begin(out_vec),
      count,
      [input_array, &index]() {
        return input_array[index++];
      });
  }
}
}
}
}