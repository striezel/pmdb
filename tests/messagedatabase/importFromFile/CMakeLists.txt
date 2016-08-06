# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for Config creation test
project(importFromFile_test)

set(importFromFile_test_src
    ../../../code/FolderMap.cpp
    ../../../code/MessageDatabase.cpp
    ../../../code/MsgTemplate.cpp
    ../../../code/PMSource.cpp
    ../../../code/PrivateMessage.cpp
    ../../../code/SortType.cpp
    ../../../code/XMLDocument.cpp
    ../../../code/XMLNode.cpp
    ../../../libstriezel/common/DirectoryFileList.cpp
    ../../../libstriezel/common/StringUtils.cpp
    ../../../libstriezel/encoding/StringConversion.cpp
    ../../../libstriezel/filesystem/directory.cpp
    ../../../libstriezel/hash/sha256/sha256.cpp
    ../../../libstriezel/hash/sha256/BufferSource.cpp
    ../../../libstriezel/hash/sha256/BufferSourceUtility.cpp
    ../../../libstriezel/hash/sha256/MessageSource.cpp
    importFromFile_test.cpp)

# We use a C++11 feature in the code, so enable C++0x a.k.a. C++11.
add_definitions(-Wall -O3 -fexceptions -std=c++0x -DNO_PM_COMPRESSION)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(importFromFile_test ${importFromFile_test_src})

# find libxml2
set(LIBXML2_DIR "../../../cmake/" )
find_package (LIBXML2)
if (LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIRS})
  target_link_libraries (importFromFile_test ${LIBXML2_LIBRARIES})
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)


# --- add it as a test
add_test(NAME class_MessageDatabase_importFromFile_single
         COMMAND $<TARGET_FILE:importFromFile_test> ${CMAKE_CURRENT_SOURCE_DIR})
