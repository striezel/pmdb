# We might support earlier versions, too, but those are not tested.
cmake_minimum_required (VERSION 3.8)

# binary for PrivateMessage::normalise() test
project(PM_normalise_test)

set(PM_normalise_test_src
    ../../../code/PMSource.cpp
    ../../../code/PrivateMessage.cpp
    ../../../libstriezel/common/StringUtils.cpp
    ../../../libstriezel/hash/sha256/sha256.cpp
    ../../../libstriezel/hash/sha256/MessageSource.cpp
    normalise.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

# add definition to avoid PM compression and its library dependency
add_definitions(-DNO_PM_COMPRESSION)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(PM_normalise_test ${PM_normalise_test_src})


# --- add it as a test
add_test(NAME class_PrivateMessage_normalise
         COMMAND $<TARGET_FILE:PM_normalise_test>)
