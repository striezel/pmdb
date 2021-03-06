# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for Config creation test
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

add_definitions(-Wall -O2 -fexceptions -DNO_PM_COMPRESSION -std=c++0x)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(addMessage_test ${addMessage_test_src})

# find libxml2
set(LIBXML2_DIR "../../../cmake/" )
find_package (LIBXML2)
if (LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIRS})
  target_link_libraries (addMessage_test ${LIBXML2_LIBRARIES})
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)


# --- add it as a test
add_test(NAME class_MessageDatabase_addMessage
         COMMAND $<TARGET_FILE:addMessage_test> ${CMAKE_CURRENT_SOURCE_DIR})
