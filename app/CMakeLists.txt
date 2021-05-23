cmake_minimum_required(VERSION 3.10)
project(l2sg LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(l2sg_app
    main.cpp
)

target_link_libraries(l2sg_app PRIVATE
    l2sg_lib
)
