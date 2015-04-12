# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for FolderMap add-get test
project(FolderMap_addget_test)

set(FolderMap_addget_test_src
    ../../../code/FolderMap.cpp
    ../../../libthoro/common/StringUtils.cpp
    ../../../libthoro/hash/sha-256.cpp
    ../../../libthoro/hash/sha-256_sources.cpp
    add-get.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(FolderMap_addget_test ${FolderMap_addget_test_src})

# --- add it as a test
add_test(NAME class_FolderMap_addget
         COMMAND $<TARGET_FILE:FolderMap_addget_test>)
