cmake_minimum_required(VERSION 3.20)
add_library(glad STATIC glad.c)
install(TARGETS glad LIBRARY DESTINATION lib INCLUDES DESTINATION include)