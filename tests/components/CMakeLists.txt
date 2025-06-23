cmake_minimum_required (VERSION 3.8...3.31)

# binary for component tests
project(component_tests)

set(component_tests_sources
    ../../code/Config.cpp
    ../../code/ConsoleColours.cpp
    ../../code/FolderMap.cpp
    ../../code/MsgTemplate.cpp
    ../../code/PMSource.cpp
    ../../code/PrivateMessage.cpp
    ../../code/bbcode/AdvancedTemplateBBCode.cpp
    ../../code/bbcode/CustomizedSimpleBBCode.cpp
    ../../code/bbcode/HorizontalRuleBBCode.cpp
    ../../code/bbcode/ListBBCode.cpp
    ../../code/bbcode/SimpleBBCode.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../code/bbcode/Smilie.cpp
    ../../code/bbcode/TableBBCode.cpp
    ../../code/filters/FilterUser.cpp
    ../../code/paths.cpp
    ../../libstriezel/common/StringUtils.cpp
    ../../libstriezel/filesystem/directory.cpp
    ../../libstriezel/hash/sha256/MessageSource.cpp
    ../../libstriezel/hash/sha256/sha256.cpp
    ../../libstriezel/zlib/CompressionFunctions.cpp
    Config.cpp
    FolderMap.cpp
    bbcode/AdvancedTemplateBBCode.cpp
    bbcode/AdvancedTplAmpTransformBBCode.cpp
    bbcode/CustomizedSimpleBBCode.cpp
    bbcode/HorizontalRuleBBCode.cpp
    bbcode/ListBBCode.cpp
    bbcode/SimpleBBCode.cpp
    bbcode/SimpleTemplateBBCode.cpp
    bbcode/SimpleTplAmpTransformBBCode.cpp
    bbcode/Smilie.cpp
    bbcode/TableBBCode.cpp
    filter/FilterUser.cpp
    names_to_controlsequences.cpp
    paths.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O2 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

# code uses some C++17 features, enable it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(component_tests ${component_tests_sources})

# MSYS2 / MinGW uses Catch 3.x.
if (HAS_CATCH_V3)
    find_package(Catch2 3 REQUIRED)
    target_link_libraries(component_tests Catch2::Catch2WithMain)
endif ()

# --- add it as a test
add_test(NAME component_testing
         COMMAND $<TARGET_FILE:component_tests>)

# find zlib
find_package (PkgConfig)
pkg_search_module (ZLIB REQUIRED zlib)
if (ZLIB_FOUND)
  if (ENABLE_STATIC_LINKING)
    include_directories(${ZLIB_STATIC_INCLUDE_DIRS})
    target_link_libraries (component_tests ${ZLIB_STATIC_LIBRARIES})
  else ()
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (component_tests ${ZLIB_LIBRARIES})
  endif ()
else ()
  message ( FATAL_ERROR "zlib was not found!" )
endif (ZLIB_FOUND)
