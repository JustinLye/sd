#pragma once

#if !defined(SD_OPSYS_UNKNOWN)
#define SD_OPSYS_UNKNOWN
#endif
#if !defined(SD_OPSYS_WINDOWS)
#define SD_OPSYS_WINDOWS 1
#endif
#if !defined(SD_OPSYS_LINUX)
#define SD_OPSYS_LINUX 2
#endif

#if !defined(SD_OPSYS)
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64) 
#define SD_OPSYS SD_OPSYS_WINDOWS
#elif defined(linux) || defined(unix) || defined(_linux) || defined(__linux) || defined(__linux__) || defined (_UNIX)
#define SD_OPSYS SD_OPSYS_LINUX
#else
#define SD_OPSYS SD_OPSYS_UNKNOWN
#endif
#endif


#include <filesystem>

#if SD_OPSYS == SD_OPSYS_WINDOWS
#include <Windows.h>
#elif SD_OPSYS == SD_OPSYS_LINUX
#include <stdlib.h>
#endif


#include "framework_api.h"

namespace sd {
namespace framework {
  struct environment_vars {
    static constexpr const char* root_directory = "SD_ROOT";
  };
  struct SD_FRAMEWORK_API program_support {
    enum class OperatingSystem : unsigned int {
      WINDOWS,
      LINUX,
      UNKNOWN
    };

    constexpr OperatingSystem OpSys() {
#if SD_OPSYS == SD_OPSYS_WINDOWS
      return OperatingSystem::WINDOWS;
#elif SD_OPSYS == SD_OPSYS_LINUX
      return OperatingSystem::LINUX;
#elif SD_OPSYS == SD_OPSYS_UNKNOWN
      return OperatingSystem::UNKNOWN;
#else
      return OperatingSystem::UNKNOWN;
#endif
    }

    static const char* getenv(const char* name) {
#if SD_OPSYS == SD_OPSYS_WINDOWS
      const DWORD buffer_size = 65535;
      static char buffer[buffer_size];
      if (GetEnvironmentVariableA(name, buffer, buffer_size))
        return buffer;
      return nullptr;
#elif SD_OPSYS == SD_OPSYS_LINUX
      return std::getenv(name);
#else
      return nullptr;
#endif
    }

    static const std::filesystem::path sd_root;
  };
}
}