cmake_minimum_required(VERSION 3.10)
project(l2sg LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS "-std=c++17")

add_library(l2sg_lib STATIC
# Source
    src/Grid.cpp
    src/Solver.cpp
    src/Generator.cpp
    src/Enums.cpp
    src/Logs.cpp
    src/techniques/NakedSingles.cpp
    src/techniques/HiddenSingles.cpp
    src/techniques/LockedCandidates.cpp
    src/techniques/XWings.cpp
    src/techniques/XyWing.cpp
    src/techniques/WWing.cpp
    src/techniques/BasicFish.cpp
    src/techniques/HiddenMulti.cpp
    src/techniques/NakedMulti.cpp
    src/techniques/Skyscraper.cpp
    src/techniques/TwoStringKite.cpp
    src/techniques/SimpleGuess.cpp
    src/techniques/BruteForce.cpp

# Private headers
    src/Utils.h

# Public headers
    include/l2sg/Cell.h
    include/l2sg/Grid.h
    include/l2sg/Solver.h
    include/l2sg/Generator.h
    include/l2sg/Enums.h
    include/l2sg/Logs.h
)

target_include_directories(l2sg_lib
    PRIVATE
        src
        include/l2sg
    PUBLIC
        include
)
