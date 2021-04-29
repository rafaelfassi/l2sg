cmake_minimum_required(VERSION 3.10)
project(sudoku LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-std=c++17")

function(add_to_test _name _sources)
    add_executable(${_name} ${_sources})
    target_link_libraries(${_name} PRIVATE sudoku_lib)
    add_test(NAME ${_name} COMMAND ${_name})
endfunction()

add_to_test(test_levels TestSolveLevel.cpp)
add_to_test(test_xwings TestXWings.cpp)
add_to_test(test_hidden_multi TestHiddenMulti.cpp)
add_to_test(test_swordfish TestSwordfish.cpp)
