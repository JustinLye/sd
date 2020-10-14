#include "program_support.h"

namespace sd {
  namespace framework {
  const std::filesystem::path program_support::sd_root = std::filesystem::path::path(program_support::getenv(environment_vars::root_directory));
  }
}