# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

#like main project, but without PM compression capabilities
project(pmdb_no_comp)

# add definition to avoid PM compression
add_definitions(-DNO_PM_COMPRESSION)

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
    ../libthoro/common/DirectoryFileList.cpp
    ../libthoro/common/StringUtils.cpp
    ../libthoro/filesystem/DirectoryFunctions.cpp
    ../libthoro/filesystem/FileFunctions.cpp
    ../libthoro/hash/sha256/sha256.cpp
    ../libthoro/hash/sha256/BufferSource.cpp
    ../libthoro/hash/sha256/BufferSourceUtility.cpp
    ../libthoro/hash/sha256/MessageSource.cpp
    ../code/main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(pmdb_no_comp ${pmdb_no_comp_sources})

# find libxml2
set(LIBXML2_DIR "../cmake/" )
find_package (LIBXML2)
if (LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIRS})
  target_link_libraries (pmdb_no_comp ${LIBXML2_LIBRARIES})
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)
