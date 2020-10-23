#pragma once

#if !defined(SD_FRAMEWORK_API)
#if defined(SD_FRAMEWORK_BUILDDLL)
#define SD_FRAMEWORK_API __declspec(dllexport)
#else
#define SD_FRAMEWORK_API 
#endif
#endif