# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

#actual project
project(pmdb)

# add_definitions(-DDEFNAME1 -DDEFINE2)

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
    ../libthoro/common/DirectoryFileList.cpp
    ../libthoro/common/StringUtils.cpp
    ../libthoro/filesystem/DirectoryFunctions.cpp
    ../libthoro/filesystem/FileFunctions.cpp
    ../libthoro/hash/sha256/sha256.cpp
    ../libthoro/hash/sha256/BufferSource.cpp
    ../libthoro/hash/sha256/BufferSourceUtility.cpp
    ../libthoro/hash/sha256/MessageSource.cpp
    ../libthoro/zlib/CompressionFunctions.cpp
    main.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions (-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

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
