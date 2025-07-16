cmake_minimum_required (VERSION 3.8...3.31)

# binary for PrivateMessage::normalise() test
project(PM_normalise_test)

set(PM_normalise_test_src
    ../../../code/PMSource.cpp
    ../../../code/PrivateMessage.cpp
    ../../../libstriezel/common/StringUtils.cpp
    ../../../libstriezel/hash/sha256/sha256.cpp
    ../../../libstriezel/hash/sha256/MessageSource.cpp
    normalise.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -Wextra -Wpedantic -pedantic-errors -Wshadow -fexceptions)
    if (ENABLE_SANITIZER)
        add_definitions (-O0)
    else ()
        add_definitions (-O3)
    endif ()

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

# add definition to avoid PM compression and its library dependency
add_definitions(-DNO_PM_COMPRESSION)

add_executable(PM_normalise_test ${PM_normalise_test_src})


# --- add it as a test
add_test(NAME class_PrivateMessage_normalise
         COMMAND $<TARGET_FILE:PM_normalise_test>)
