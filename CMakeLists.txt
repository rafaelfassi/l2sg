cmake_minimum_required(VERSION 3.10)
project(sudoku LANGUAGES CXX)
include(CTest)

add_subdirectory(lib)
add_subdirectory(app)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
