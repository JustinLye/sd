cmake_minimum_required(VERSION 3.20)
add_executable(sd sd.cc)
set_target_properties(sd PROPERTIES CXX_STANDARD 20)
add_dependencies(sd libsd)
target_link_libraries(sd PRIVATE libsd)
install(TARGETS sd
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
#add_custom_command(TARGET sd POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#    "$<TARGET_RUNTIME_DLLS:glfw>"
#    "$<TARGET_FILE_DIR:sd"
#)