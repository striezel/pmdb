# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for tests of SimpleBBCode class
project(simple_bbcode_test)

set(simple_bbcode_test_src
    ../../code/bbcode/SimpleBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    simple_bbcode_test.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(simple_bbcode_test ${simple_bbcode_test_src})

# --- add it as a test
add_test(NAME class_SimpleBBCode
         COMMAND $<TARGET_FILE:simple_bbcode_test>)


# binary for tests of CustomizedSimpleBBCode class
project(customized_simple_bbcode_test)

set(customized_simple_bbcode_test_src
    ../../code/bbcode/CustomizedSimpleBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    customized_simple_bbcode_test.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(customized_simple_bbcode_test ${customized_simple_bbcode_test_src})

# --- add it as a test
add_test(NAME class_CustomizedSimpleBBCode
         COMMAND $<TARGET_FILE:customized_simple_bbcode_test>)


# binary for tests of HorizontalBBCode class
project(horizontal_rule_bbcode_test)

set(horizontal_rule_bbcode_test_src
    ../../code/bbcode/HorizontalRuleBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    horizontal_rule_bbcode_test.cpp)

# code uses some C++11 features, enable it via -std=c++0x
add_definitions(-Wall -O3 -fexceptions -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(horizontal_rule_bbcode_test ${horizontal_rule_bbcode_test_src})

# --- add it as a test
add_test(NAME class_HorizontalRuleBBCode
         COMMAND $<TARGET_FILE:horizontal_rule_bbcode_test>)
