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
    ../code/bbcode/quotes.cpp
    ../code/filters/FilterUser.cpp
    ../code/paths.cpp
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
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O3 -fexceptions)

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
