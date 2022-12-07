cmake_minimum_required (VERSION 3.8)

# binary for Config tests
project(config_tests)

set(config_tests_sources
    ../../code/Config.cpp
    ../../code/bbcode/Smilie.cpp
    Config.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -O3 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(config_tests ${config_tests_sources})

# --- add it as a test
add_test(NAME Config_tests
         COMMAND $<TARGET_FILE:config_tests>)
