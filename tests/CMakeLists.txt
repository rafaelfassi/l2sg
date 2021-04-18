cmake_minimum_required(VERSION 3.10)

project(tests LANGUAGES CXX)
enable_testing()

SET(CMAKE_CXX_FLAGS "-std=c++17")

add_executable(all_tests
    testsolverlogic.cpp
)

target_include_directories(all_tests
	PRIVATE
		${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(all_tests
	PUBLIC
    sudoku_solver_lib
)

add_test(NAME all_tests COMMAND all_tests)

