cmake_minimum_required(VERSION 2.5)
project(gfwx-compress)

set(SOURCE_FILES main.cpp)

add_executable(gfwx-compress ${SOURCE_FILES})
target_link_libraries(gfwx "gfwx")