cmake_minimum_required(VERSION 3.20)
add_library(libsd STATIC)
add_dependencies(libsd glad glfw)
target_link_libraries(libsd PUBLIC glad glfw)
target_compile_definitions(libsd PUBLIC GLFW_INCLUDE_NONE)
set_target_properties(libsd PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(libsd PROPERTIES CXX_STANDARD 20)

set (LIBSD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(libsd PUBLIC "${LIBSD_INCLUDE_DIR}")

set (LIBSD_FRAMEWORK_DIR ${LIBSD_INCLUDE_DIR}/sd/framework)
set (LIBSD_FRAMEWORK_INPUT_DIR ${LIBSD_FRAMEWORK_DIR}/input)
set (LIBSD_GAMEPLAY_DIR ${LIBSD_INCLUDE_DIR}/sd/gameplay)
set (LIBSD_FRAMEWORK_INTERFACES_DIR ${LIBSD_FRAMEWORK_DIR}/interfaces)
set (LIBSD_FRAMEWORK_TIME_DIR ${LIBSD_FRAMEWORK_DIR}/time)

set (LIBSD_FRAMEWORK_TIME_HEADERS
    "${LIBSD_FRAMEWORK_TIME_DIR}/clock.h"
)

set (LIBSD_FRAMEWORK_INPUT_HEADERS
    "${LIBSD_FRAMEWORK_INPUT_DIR}/key_state_type.h"
    "${LIBSD_FRAMEWORK_INPUT_DIR}/key_state.h"
    "${LIBSD_FRAMEWORK_INPUT_DIR}/key_tracker.h"
    "${LIBSD_FRAMEWORK_INPUT_DIR}/key.h"
)

set (LIBSD_GAMEPLAY_HEADERS
    "${LIBSD_GAMEPLAY_DIR}/world.h"
)

set (LIBSD_FRAMEWORK_INTERFACES_HEADERS
    "${LIBSD_FRAMEWORK_INTERFACES_DIR}/IComponent.h"
)

set (LIBSD_FRAMEWORK_TIME_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/clock.cc"
)

set (LIBSD_FRAMEWORK_INPUT_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/key_state.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/key_tracker.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/key.cc"
)

set (LIBSD_GAMEPLAY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/world.cc"
)

target_sources(
    libsd
    PRIVATE
       ${LIBSD_FRAMEWORK_TIME_HEADERS}
       ${LIBSD_FRAMEWORK_TIME_SOURCES}
       ${LIBSD_FRAMEWORK_INPUT_HEADERS}
       ${LIBSD_FRAMEWORK_INPUT_SOURCES}
       ${LIBSD_FRAMEWORK_INTERFACES_HEADERS}
       ${LIBSD_GAMEPLAY_HEADERS}
       ${LIBSD_GAMEPLAY_SOURCES}
)

source_group(
    "framework\\time"
    FILES
        ${LIBSD_FRAMEWORK_TIME_HEADERS}
        ${LIBSD_FRAMEWORK_TIME_SOURCES}
)

source_group(
    "framework\\input"
    FILES
        ${LIBSD_FRAMEWORK_INPUT_HEADERS}
        ${LIBSD_FRAMEWORK_INPUT_SOURCES}
)

source_group(
    "framework\\interfaces"
    FILES
        ${LIBSD_FRAMEWORK_INTERFACES_HEADERS}   
)

source_group(
    "gameplay"
    FILES
        ${LIBSD_GAMEPLAY_HEADERS}
        ${LIBSD_GAMEPLAY_SOURCES}   
)