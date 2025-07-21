cmake_minimum_required (VERSION 3.8...3.31)

# binary for tests of MessageDatabase::addMessage()
# TODO: move to component tests
project(addMessage_test)

set(addMessage_test_src
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
    ../../../libstriezel/filesystem/directory.cpp
    ../../../libstriezel/hash/sha256/sha256.cpp
    ../../../libstriezel/hash/sha256/BufferSource.cpp
    ../../../libstriezel/hash/sha256/BufferSourceUtility.cpp
    ../../../libstriezel/hash/sha256/MessageSource.cpp
    addMessage.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
  if (ENABLE_SANITIZER OR CODE_COVERAGE)
    add_definitions (-O0)
  else ()
    add_definitions (-O2)
  endif ()

  set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()
add_definitions(-DNO_PM_COMPRESSION)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(addMessage_test ${addMessage_test_src})

# find libxml2
find_package (PkgConfig)
pkg_search_module (LIBXML2 REQUIRED libxml-2.0)
if (LIBXML2_FOUND)
  if (ENABLE_STATIC_LINKING)
    include_directories(${LIBXML2_STATIC_INCLUDE_DIRS})
    target_link_libraries (addMessage_test ${LIBXML2_STATIC_LIBRARIES})
  else ()
    include_directories(${LIBXML2_INCLUDE_DIRS})
    target_link_libraries (addMessage_test ${LIBXML2_LIBRARIES})
  endif ()
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)


# --- add it as a test
add_test(NAME class_MessageDatabase_addMessage
         COMMAND $<TARGET_FILE:addMessage_test> "${CMAKE_CURRENT_SOURCE_DIR}")
