cmake_minimum_required (VERSION 3.8)

# binary for FolderMap add-get test
project(FolderMap_addget_test)

set(FolderMap_addget_test_src
    ../../../code/FolderMap.cpp
    ../../../libstriezel/common/StringUtils.cpp
    ../../../libstriezel/hash/sha256/sha256.cpp
    ../../../libstriezel/hash/sha256/MessageSource.cpp
    add-get.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(FolderMap_addget_test ${FolderMap_addget_test_src})

# --- add it as a test
add_test(NAME class_FolderMap_addget
         COMMAND $<TARGET_FILE:FolderMap_addget_test>)
