cmake_minimum_required (VERSION 3.8)

# binary for tests of SimpleBBCode class
project(BBCode_tests)

set(BBCode_tests_sources
    ../../code/MsgTemplate.cpp
    ../../code/bbcode/AdvancedTemplateBBCode.cpp
    ../../code/bbcode/CustomizedSimpleBBCode.cpp
    ../../code/bbcode/HorizontalRuleBBCode.cpp
    ../../code/bbcode/ListBBCode.cpp
    ../../code/bbcode/SimpleBBCode.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../code/bbcode/Smilie.cpp
    ../../code/bbcode/TableBBCode.cpp
    ../../libstriezel/common/StringUtils.cpp
    AdvancedTemplateBBCode.cpp
    AdvancedTplAmpTransformBBCode.cpp
    CustomizedSimpleBBCode.cpp
    HorizontalRuleBBCode.cpp
    ListBBCode.cpp
    SimpleBBCode.cpp
    SimpleTemplateBBCode.cpp
    SimpleTplAmpTransformBBCode.cpp
    Smilie.cpp
    TableBBCode.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

# code uses some C++17 features, enable it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSYS2 / MinGW uses Catch 3.x.
if (HAS_CATCH_V3)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(BBCode_tests Catch2::Catch2WithMain)
endif ()

add_executable(BBCode_tests ${BBCode_tests_sources})

# --- add it as a test
add_test(NAME BBCode_testing
         COMMAND $<TARGET_FILE:BBCode_tests>)
