cmake_minimum_required (VERSION 3.8)

# binary for tests of SimpleBBCode class
project(SimpleBBCode_test)

set(SimpleBBCode_test_src
    ../../code/bbcode/SimpleBBCode.cpp
    ../../libstriezel/common/StringUtils.cpp
    SimpleBBCode_test.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(SimpleBBCode_test ${SimpleBBCode_test_src})

# --- add it as a test
add_test(NAME class_SimpleBBCode
         COMMAND $<TARGET_FILE:SimpleBBCode_test>)


# binary for tests of CustomizedSimpleBBCode class
project(CustomizedSimpleBBCode_test)

set(CustomizedSimpleBBCode_test_src
    ../../code/bbcode/CustomizedSimpleBBCode.cpp
    ../../libstriezel/common/StringUtils.cpp
    CustomizedSimpleBBCode_test.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -O3 -fexceptions)
endif ()

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(CustomizedSimpleBBCode_test ${CustomizedSimpleBBCode_test_src})

# --- add it as a test
add_test(NAME class_CustomizedSimpleBBCode
         COMMAND $<TARGET_FILE:CustomizedSimpleBBCode_test>)


# binary for tests of HorizontalRuleBBCode class
project(HorizontalRuleBBCode_test)

set(HorizontalRuleBBCode_test_src
    ../../code/bbcode/HorizontalRuleBBCode.cpp
    ../../libstriezel/common/StringUtils.cpp
    HorizontalRuleBBCode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)
# code uses some C++11 features, enable it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(HorizontalRuleBBCode_test ${HorizontalRuleBBCode_test_src})

# --- add it as a test
add_test(NAME class_HorizontalRuleBBCode
         COMMAND $<TARGET_FILE:HorizontalRuleBBCode_test>)


# binary for tests of ListBBCode class
project(ListBBCode_test)

set(ListBBCode_test_src
    ../../code/bbcode/ListBBCode.cpp
    ../../libstriezel/common/StringUtils.cpp
    ListBBCode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(ListBBCode_test ${ListBBCode_test_src})

# --- add it as a test
add_test(NAME class_ListBBCode
         COMMAND $<TARGET_FILE:ListBBCode_test>)


# binary for tests of SimpleTemplateBBCode class
project(SimpleTemplateBBCode_test)

set(SimpleTemplateBBCode_test_src
    ../../code/MsgTemplate.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../libstriezel/common/StringUtils.cpp
    SimpleTemplateBBCode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(SimpleTemplateBBCode_test ${SimpleTemplateBBCode_test_src})

# --- add it as a test
add_test(NAME class_SimpleTemplateBBCode
         COMMAND $<TARGET_FILE:SimpleTemplateBBCode_test>)


# binary for tests of AdvancedTemplateBBCode class
project(AdvancedTemplateBBCode_test)

set(AdvancedTemplateBBCode_test_src
    ../../code/MsgTemplate.cpp
    ../../code/bbcode/AdvancedTemplateBBCode.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../libstriezel/common/StringUtils.cpp
    AdvancedTemplateBBCode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(AdvancedTemplateBBCode_test ${AdvancedTemplateBBCode_test_src})

# --- add it as a test
add_test(NAME class_AdvancedTemplateBBCode
         COMMAND $<TARGET_FILE:AdvancedTemplateBBCode_test>)


# binary for tests of SimpleTplAmpTransformBBCode class
project(SimpleTplAmpTransformBBCode_test)

set(SimpleTplAmpTransformBBCode_test_src
    ../../code/MsgTemplate.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../libstriezel/common/StringUtils.cpp
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
    ../../libstriezel/common/StringUtils.cpp
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
    ../../libstriezel/common/StringUtils.cpp
    TableBBCode_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(TableBBCode_test ${TableBBCode_test_src})

# --- add it as a test
add_test(NAME class_TableBBCode_simple
         COMMAND $<TARGET_FILE:TableBBCode_test>)


# binary for tests of Smilie class
project(Smilie_test)

set(Smilie_test_src
    ../../code/bbcode/Smilie.cpp
    Smilie_test.cpp)

add_definitions(-Wall -O3 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(Smilie_test ${Smilie_test_src})

# --- add it as a test
add_test(NAME class_Smilie
         COMMAND $<TARGET_FILE:Smilie_test>)
