cmake_minimum_required (VERSION 3.8...3.31)

#actual project
project(pmdb)

set(pmdb_sources
    Config.cpp
    ColourMap.cpp
    CompressionDetection.cpp
    ConsoleColours.cpp
    FolderMap.cpp
    MessageDatabase.cpp
    MsgTemplate.cpp
    PMSource.cpp
    PrivateMessage.cpp
    SortType.cpp
    Version.cpp
    XMLDocument.cpp
    XMLNode.cpp
    bbcode/AdvancedTemplateBBCode.cpp
    bbcode/BBCodeParser.cpp
    bbcode/CustomizedSimpleBBCode.cpp
    bbcode/HorizontalRuleBBCode.cpp
    bbcode/ListBBCode.cpp
    bbcode/SimpleBBCode.cpp
    bbcode/SimpleTemplateBBCode.cpp
    bbcode/Smilie.cpp
    bbcode/TableBBCode.cpp
    bbcode/TableClasses.cpp
    bbcode/quotes.cpp
    filters/FilterUser.cpp
    functions.cpp
    html_generation.cpp
    paths.cpp
    templates/defaults.hpp
    templates/functions.cpp
    ../libstriezel/common/DirectoryFileList.cpp
    ../libstriezel/common/StringUtils.cpp
    ../libstriezel/filesystem/directory.cpp
    ../libstriezel/filesystem/file.cpp
    ../libstriezel/hash/sha256/sha256.cpp
    ../libstriezel/hash/sha256/BufferSource.cpp
    ../libstriezel/hash/sha256/BufferSourceUtility.cpp
    ../libstriezel/hash/sha256/MessageSource.cpp
    ../libstriezel/zlib/CompressionFunctions.cpp
    main.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (ENABLE_SANITIZER OR CODE_COVERAGE)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pmdb ${pmdb_sources})

# find libxml2
find_package (PkgConfig)
pkg_search_module (LIBXML2 REQUIRED libxml-2.0)
if (LIBXML2_FOUND)
  if (ENABLE_STATIC_LINKING)
    include_directories(${LIBXML2_STATIC_INCLUDE_DIRS})
    target_link_libraries (pmdb ${LIBXML2_STATIC_LIBRARIES})
  else ()
    include_directories(${LIBXML2_INCLUDE_DIRS})
    target_link_libraries (pmdb ${LIBXML2_LIBRARIES})
  endif ()
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)

#find zlib
pkg_search_module (ZLIB REQUIRED zlib)
if (ZLIB_FOUND)
  if (ENABLE_STATIC_LINKING)
    include_directories(${ZLIB_STATIC_INCLUDE_DIRS})
    target_link_libraries (pmdb ${ZLIB_STATIC_LIBRARIES})
  else ()
    include_directories(${ZLIB_INCLUDE_DIRS})
    target_link_libraries (pmdb ${ZLIB_LIBRARIES})
  endif ()
else ()
  message ( FATAL_ERROR "zlib was not found!" )
endif (ZLIB_FOUND)

## get the default templates
execute_process(
  COMMAND cat folder.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/templates
  OUTPUT_VARIABLE FOLDER_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat folder_entry.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/templates
  OUTPUT_VARIABLE FOLDER_ENTRY_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat folder_list.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/templates
  OUTPUT_VARIABLE FOLDER_LIST_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat index_entry.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/templates
  OUTPUT_VARIABLE INDEX_ENTRY_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND cat message.tpl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/templates
  OUTPUT_VARIABLE MESSAGE_TPL
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# replace placeholders in defaults.prototype.hpp and create defaults.hpp
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/templates/defaults.prototype.hpp
               ${CMAKE_CURRENT_SOURCE_DIR}/templates/defaults.hpp
               @ONLY)
