cmake_minimum_required (VERSION 3.8...3.31)

# binary for test of importing messages from files
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

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# no compression
add_definitions(-DNO_PM_COMPRESSION)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
  if (ENABLE_SANITIZER)
    add_definitions (-O0)
  else ()
    add_definitions (-O3)
  endif ()

  set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

add_executable(importFromFile_test ${importFromFile_test_src})

# find libxml2
find_package (PkgConfig)
pkg_search_module (LIBXML2 REQUIRED libxml-2.0)
if (LIBXML2_FOUND)
  if (ENABLE_STATIC_LINKING)
    include_directories(${LIBXML2_STATIC_INCLUDE_DIRS})
    target_link_libraries (importFromFile_test ${LIBXML2_STATIC_LIBRARIES})
  else ()
    include_directories(${LIBXML2_INCLUDE_DIRS})
    target_link_libraries (importFromFile_test ${LIBXML2_LIBRARIES})
  endif ()
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)

# find iconv
find_package(Iconv)
if (Iconv_FOUND)
  if (NOT Iconv_IS_BUILT_IN)
    include_directories(${Iconv_INCLUDE_DIRS})
    target_link_libraries (importFromFile_test ${Iconv_LIBRARIES})
  endif ()
else ()
  message ( FATAL_ERROR "libiconv was not found!" )
endif (Iconv_FOUND)

# --- add it as a test
add_test(NAME class_MessageDatabase_importFromFile_single
         COMMAND $<TARGET_FILE:importFromFile_test> "${CMAKE_CURRENT_SOURCE_DIR}")
