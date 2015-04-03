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


# binary for tests of ListBBCode class
project(list_bbcode_test)

set(list_bbcode_test_src
    ../../code/bbcode/ListBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    list_bbcode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(list_bbcode_test ${list_bbcode_test_src})

# --- add it as a test
add_test(NAME class_ListBBCode
         COMMAND $<TARGET_FILE:list_bbcode_test>)


# binary for tests of SimpleTemplateBBCode class
project(simple_template_bbcode_test)

set(simple_template_bbcode_test_src
    ../../code/MsgTemplate.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    simple_template_bbcode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(simple_template_bbcode_test ${simple_template_bbcode_test_src})

# --- add it as a test
add_test(NAME class_SimpleTemplateBBCode
         COMMAND $<TARGET_FILE:simple_template_bbcode_test>)


# binary for tests of AdvancedTemplateBBCode class
project(advanced_template_bbcode_test)

set(advanced_template_bbcode_test_src
    ../../code/MsgTemplate.cpp
    ../../code/bbcode/AdvancedTemplateBBCode.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    advanced_template_bbcode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(advanced_template_bbcode_test ${advanced_template_bbcode_test_src})

# --- add it as a test
add_test(NAME class_AdvancedTemplateBBCode
         COMMAND $<TARGET_FILE:advanced_template_bbcode_test>)


# binary for tests of SimpleTplAmpTransformBBCode class
project(SimpleTplAmpTransformBBCode_test)

set(SimpleTplAmpTransformBBCode_test_src
    ../../code/MsgTemplate.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    SimpleTplAmpTransformBBCode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(SimpleTplAmpTransformBBCode_test ${SimpleTplAmpTransformBBCode_test_src})

# --- add it as a test
add_test(NAME class_SimpleTplAmpTransformBBCode
         COMMAND $<TARGET_FILE:SimpleTplAmpTransformBBCode_test>)


# binary for tests of AdvancedTplAmpTransformBBCode class
project(AdvancedTplAmpTransformBBCode_test)

set(AdvancedTplAmpTransformBBCode_test_src
    ../../code/MsgTemplate.cpp
    ../../code/bbcode/AdvancedTemplateBBCode.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    AdvancedTplAmpTransformBBCode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(AdvancedTplAmpTransformBBCode_test ${AdvancedTplAmpTransformBBCode_test_src})

# --- add it as a test
add_test(NAME class_AdvancedTplAmpTransformBBCode
         COMMAND $<TARGET_FILE:AdvancedTplAmpTransformBBCode_test>)


# binary for tests of TableBBCode class
project(TableBBCode_test)

set(TableBBCode_test_src
    ../../code/bbcode/TableBBCode.cpp
    ../../libthoro/common/StringUtils.cpp
    TableBBCode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(TableBBCode_test ${TableBBCode_test_src})

# --- add it as a test
add_test(NAME class_TableBBCode_simple
         COMMAND $<TARGET_FILE:TableBBCode_test>)


# binary for tests of Smilie class
project(smilie_test)

set(smilie_test_src
    ../../code/bbcode/Smilie.cpp
    smilie_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(smilie_test ${smilie_test_src})

# --- add it as a test
add_test(NAME class_Smilie
         COMMAND $<TARGET_FILE:smilie_test>)
