cmake_minimum_required(VERSION 2.5)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(include)
add_subdirectory(libgfwx)
add_subdirectory(test)