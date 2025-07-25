cmake_minimum_required (VERSION 3.8...3.31)

#like main project, but without PM compression capabilities
project(pmdb_no_comp)

# add definition to avoid PM compression
add_definitions(-DNO_PM_COMPRESSION)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(pmdb_no_comp_sources
    ../code/Config.cpp
    ../code/ColourMap.cpp
    ../code/ConsoleColours.cpp
    ../code/CompressionDetection.cpp
    ../code/FolderMap.cpp
    ../code/MessageDatabase.cpp
    ../code/MsgTemplate.cpp
    ../code/PMSource.cpp
    ../code/PrivateMessage.cpp
    ../code/SortType.cpp
    ../code/Version.cpp
    ../code/XMLDocument.cpp
    ../code/XMLNode.cpp
    ../code/bbcode/AdvancedTemplateBBCode.cpp
    ../code/bbcode/BBCodeParser.cpp
    ../code/bbcode/CustomizedSimpleBBCode.cpp
    ../code/bbcode/HorizontalRuleBBCode.cpp
    ../code/bbcode/ListBBCode.cpp
    ../code/bbcode/SimpleBBCode.cpp
    ../code/bbcode/SimpleTemplateBBCode.cpp
    ../code/bbcode/Smilie.cpp
    ../code/bbcode/TableBBCode.cpp
    ../code/bbcode/TableClasses.cpp
    ../code/bbcode/quotes.cpp
    ../code/filters/FilterUser.cpp
    ../code/functions.cpp
    ../code/html_generation.cpp
    ../code/paths.cpp
    ../code/templates/defaults.hpp
    ../code/templates/functions.cpp
    ../libstriezel/common/DirectoryFileList.cpp
    ../libstriezel/common/StringUtils.cpp
    ../libstriezel/filesystem/directory.cpp
    ../libstriezel/filesystem/file.cpp
    ../libstriezel/hash/sha256/sha256.cpp
    ../libstriezel/hash/sha256/BufferSource.cpp
    ../libstriezel/hash/sha256/BufferSourceUtility.cpp
    ../libstriezel/hash/sha256/MessageSource.cpp
    ../code/main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (ENABLE_SANITIZER OR CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

add_executable(pmdb_no_comp ${pmdb_no_comp_sources})

# find libxml2
find_package (PkgConfig)
pkg_search_module (LIBXML2 REQUIRED libxml-2.0)
if (LIBXML2_FOUND)
  if (ENABLE_STATIC_LINKING)
    include_directories(${LIBXML2_STATIC_INCLUDE_DIRS})
    target_link_libraries (pmdb_no_comp ${LIBXML2_STATIC_LIBRARIES})
  else ()
    include_directories(${LIBXML2_INCLUDE_DIRS})
    target_link_libraries (pmdb_no_comp ${LIBXML2_LIBRARIES})
  endif ()
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)

# get the default templates
execute_process(
  COMMAND cat folder.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../code/templates
  OUTPUT_VARIABLE FOLDER_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat folder_entry.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../code/templates
  OUTPUT_VARIABLE FOLDER_ENTRY_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat folder_list.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../code/templates
  OUTPUT_VARIABLE FOLDER_LIST_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat index_entry.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../code/templates
  OUTPUT_VARIABLE INDEX_ENTRY_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat message.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../code/templates
  OUTPUT_VARIABLE MESSAGE_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# replace placeholders in defaults.prototype.hpp and create defaults.hpp
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../code/templates/defaults.prototype.hpp
               ${CMAKE_CURRENT_SOURCE_DIR}/../code/templates/defaults.hpp
               @ONLY)
