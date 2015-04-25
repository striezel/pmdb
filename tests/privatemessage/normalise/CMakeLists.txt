# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for PrivateMessage::normalise() test
project(PM_normalise_test)

set(PM_normalise_test_src
    ../../../code/PMSource.cpp
    ../../../code/PrivateMessage.cpp
    ../../../libthoro/common/StringUtils.cpp
    ../../../libthoro/hash/sha-256.cpp
    ../../../libthoro/hash/sha-256_sources.cpp
    ../../../libthoro/zlib/CompressionFunctions.cpp
    normalise.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(PM_normalise_test ${PM_normalise_test_src})

#find zlib
find_package (ZLIB)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries (PM_normalise_test ${ZLIB_LIBRARIES})
else ()
  message ( FATAL_ERROR "zlib was not found!" )
endif (ZLIB_FOUND)

# --- add it as a test
add_test(NAME class_PrivateMessage_normalise
         COMMAND $<TARGET_FILE:PM_normalise_test>)
