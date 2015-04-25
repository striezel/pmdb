# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for Config creation test
project(MessageDatabase_saveload_test)

set(MessageDatabase_saveload_test_src
    ../../../code/FolderMap.cpp
    ../../../code/MessageDatabase.cpp
    ../../../code/MsgTemplate.cpp
    ../../../code/PMSource.cpp
    ../../../code/PrivateMessage.cpp
    ../../../code/SortType.cpp
    ../../../code/XMLDocument.cpp
    ../../../code/XMLNode.cpp
    ../../../libthoro/common/DirectoryFileList.cpp
    ../../../libthoro/common/StringUtils.cpp
    ../../../libthoro/filesystem/DirectoryFunctions.cpp
    ../../../libthoro/hash/sha-256.cpp
    ../../../libthoro/hash/sha-256_sources.cpp
    ../../../libthoro/zlib/CompressionFunctions.cpp
    save-load-test.cpp)

add_definitions(-Wall -O2 -fexceptions)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(MessageDatabase_saveload_test ${MessageDatabase_saveload_test_src})

# find libxml2
set(LIBXML2_DIR "../../../cmake/" )
find_package (LIBXML2)
if (LIBXML2_FOUND)
  include_directories(${LIBXML2_INCLUDE_DIRS})
  target_link_libraries (MessageDatabase_saveload_test ${LIBXML2_LIBRARIES})
else ()
  message ( FATAL_ERROR "libxml2 was not found!" )
endif (LIBXML2_FOUND)


# --- add it as a test
add_test(NAME class_MessageDatabase_save_load
         COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/save-load-test.sh $<TARGET_FILE:MessageDatabase_saveload_test>)
