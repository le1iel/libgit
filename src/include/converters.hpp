#include <cstdlib>
#include <gitxx/diff_delta.hpp>
#include <gitxx/error.hpp>

namespace gitxx::internal {

template <typename CppType, typename CType>
struct conversion_traits;

// TODO How to move these to the cpp files
template <>
struct conversion_traits<GitErrc, int> {
  static GitErrc from_c(const int& error) {
    return static_cast<GitErrc>(std::abs(error));
  }
  static int from_cpp(const GitErrc& error){
      return static_cast<int>(error);
  }
};

}  // namespace gitxx::internal
