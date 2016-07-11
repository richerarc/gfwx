cmake_minimum_required(VERSION 2.5)
project(gfwx-compress)

find_package( OpenCV REQUIRED )

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(SOURCE_FILES main.cpp)

add_executable(gfwx-compress ${SOURCE_FILES})
target_link_libraries(gfwx-compress "gfwx")
target_link_libraries(gfwx-compress ${OpenCV_LIBS})

