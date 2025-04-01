cmake_minimum_required (VERSION 3.8...3.31)

#actual project
project(pmdb)

set(pmdb_sources
    Config.cpp
    ColourMap.cpp
    ConsoleColours.cpp
    FolderMap.cpp
    MessageDatabase.cpp
    MsgTemplate.cpp
    PMSource.cpp
    PrivateMessage.cpp
    SortType.cpp
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
    bbcode/quotes.cpp
    filters/FilterUser.cpp
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
    add_definitions (-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -O3 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pmdb ${pmdb_sources})

# find libxml2
set(LIBXML2_DIR "../cmake/" )
find_package (LIBXML2)
if (LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIRS})
  target_link_libraries (pmdb ${LIBXML2_LIBRARIES})
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)

#find zlib
find_package (ZLIB)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries (pmdb ${ZLIB_LIBRARIES})
else ()
  message ( FATAL_ERROR "zlib was not found!" )
endif (ZLIB_FOUND)
