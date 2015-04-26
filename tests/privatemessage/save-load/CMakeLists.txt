# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for PrivateMessage::saveToFile()/loadFromFile() test
project(PM_save_load_test)

set(PM_save_load_test_src
    ../../../code/PMSource.cpp
    ../../../code/PrivateMessage.cpp
    ../../../libthoro/common/StringUtils.cpp
    ../../../libthoro/filesystem/FileFunctions.cpp
    ../../../libthoro/hash/sha-256.cpp
    ../../../libthoro/hash/sha-256_sources.cpp
    save-load.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

# add definition to avoid PM compression (this test is for uncompressed data)
add_definitions(-DNO_PM_COMPRESSION)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(PM_save_load_test ${PM_save_load_test_src})

# --- add it as a test
add_test(NAME class_PrivateMessage_save_load
         COMMAND $<TARGET_FILE:PM_save_load_test>)
