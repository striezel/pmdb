cmake_minimum_required (VERSION 3.8...3.31)

# binary for component tests
project(component_tests)

set(component_tests_sources
    ../../code/CompressionDetection.cpp
    ../../code/Config.cpp
    ../../code/ConsoleColours.cpp
    ../../code/FolderMap.cpp
    ../../code/MessageDatabase.cpp
    ../../code/MsgTemplate.cpp
    ../../code/PMSource.cpp
    ../../code/PrivateMessage.cpp
    ../../code/SortType.cpp
    ../../code/XMLDocument.cpp
    ../../code/XMLNode.cpp
    ../../code/bbcode/AdvancedTemplateBBCode.cpp
    ../../code/bbcode/CustomizedSimpleBBCode.cpp
    ../../code/bbcode/HorizontalRuleBBCode.cpp
    ../../code/bbcode/ListBBCode.cpp
    ../../code/bbcode/SimpleBBCode.cpp
    ../../code/bbcode/SimpleTemplateBBCode.cpp
    ../../code/bbcode/Smilie.cpp
    ../../code/bbcode/TableBBCode.cpp
    ../../code/bbcode/TableClasses.cpp
    ../../code/bbcode/TextProcessor.hpp
    ../../code/bbcode/quotes.cpp
    ../../code/filters/FilterUser.cpp
    ../../code/paths.cpp
    ../../code/templates/defaults.hpp
    ../../libstriezel/common/DirectoryFileList.cpp
    ../../libstriezel/common/StringUtils.cpp
    ../../libstriezel/filesystem/directory.cpp
    ../../libstriezel/hash/sha256/BufferSource.cpp
    ../../libstriezel/hash/sha256/BufferSourceUtility.cpp
    ../../libstriezel/hash/sha256/MessageSource.cpp
    ../../libstriezel/hash/sha256/sha256.cpp
    ../../libstriezel/zlib/CompressionFunctions.cpp
    ../FileGuard.hpp
    CompressionDetections.cpp
    Config.cpp
    FolderMap.cpp
    MessageDatabase.cpp
    SortType.cpp
    bbcode/AdvancedTemplateBBCode.cpp
    bbcode/AdvancedTplAmpTransformBBCode.cpp
    bbcode/CustomizedSimpleBBCode.cpp
    bbcode/HorizontalRuleBBCode.cpp
    bbcode/KillSpacesBeforeNewline.cpp
    bbcode/ListBBCode.cpp
    bbcode/ListNewlinePreProcessor.cpp
    bbcode/SimpleBBCode.cpp
    bbcode/SimpleTemplateBBCode.cpp
    bbcode/SimpleTplAmpTransformBBCode.cpp
    bbcode/Smilie.cpp
    bbcode/TableBBCode.cpp
    bbcode/TablePreProcessor.cpp
    bbcode/quotes.cpp
    filter/FilterUser.cpp
    names_to_controlsequences.cpp
    paths.cpp
    templates/defaults.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (ENABLE_SANITIZER OR CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O2)
    endif ()

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

# find libxml2
pkg_search_module (LIBXML2 REQUIRED libxml-2.0)
if (LIBXML2_FOUND)
  if (ENABLE_STATIC_LINKING)
    include_directories(${LIBXML2_STATIC_INCLUDE_DIRS})
    target_link_libraries (component_tests ${LIBXML2_STATIC_LIBRARIES})
  else ()
    include_directories(${LIBXML2_INCLUDE_DIRS})
    target_link_libraries (component_tests ${LIBXML2_LIBRARIES})
  endif ()
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)

# GNU GCC before 9.1.0 needs to link to libstdc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.1.0")
  target_link_libraries(component_tests stdc++fs)
endif ()

# Clang before 9.0 needs to link to libc++fs explicitly.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
  if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "8.0")
    # If we are on Clang 7.x, then the filesystem library from GCC is better.
    target_link_libraries(component_tests stdc++fs)
  else ()
    # Use Clang's C++ filesystem library, it is recent enough.
    target_link_libraries(component_tests c++fs)
  endif ()
endif ()

# get the default templates
execute_process(
  COMMAND cat folder.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../code/templates
  OUTPUT_VARIABLE FOLDER_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat folder_entry.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../code/templates
  OUTPUT_VARIABLE FOLDER_ENTRY_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat folder_list.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../code/templates
  OUTPUT_VARIABLE FOLDER_LIST_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat index_entry.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../code/templates
  OUTPUT_VARIABLE INDEX_ENTRY_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat message.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../code/templates
  OUTPUT_VARIABLE MESSAGE_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# replace placeholders in defaults.prototype.hpp and create defaults.hpp
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../code/templates/defaults.prototype.hpp
               ${CMAKE_CURRENT_SOURCE_DIR}/../../code/templates/defaults.hpp
               @ONLY)
