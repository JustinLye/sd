#pragma once
#if !defined(SD_GRAPHICS_API)
#if defined(SD_GRAPHICS_BUILDDLL)
#define SD_GRAPHICS_API __declspec(dllexport)
#else
#define SD_GRAPHICS_API  
#endif
#endif