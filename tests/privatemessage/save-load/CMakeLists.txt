cmake_minimum_required (VERSION 3.8)

# binary for PrivateMessage::saveToFile()/loadFromFile() test
project(PM_save_load_test)

set(PM_save_load_test_src
    ../../../code/PMSource.cpp
    ../../../code/PrivateMessage.cpp
    ../../../libstriezel/common/StringUtils.cpp
    ../../../libstriezel/filesystem/file.cpp
    ../../../libstriezel/hash/sha256/sha256.cpp
    ../../../libstriezel/hash/sha256/MessageSource.cpp
    save-load.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif (CMAKE_COMPILER_IS_GNUCC)

# add definition to avoid PM compression (this test is for uncompressed data)
add_definitions(-DNO_PM_COMPRESSION)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PM_save_load_test ${PM_save_load_test_src})

# --- add it as a test
add_test(NAME class_PrivateMessage_save_load
         COMMAND $<TARGET_FILE:PM_save_load_test>)
