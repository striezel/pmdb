cmake_minimum_required (VERSION 3.8...3.31)

# binary for PrivateMessage::saveToFile()/loadFromFile() test
project(PM_save_load_compressed_test)

set(PM_save_load_compressed_test_src
    ../../../code/PMSource.cpp
    ../../../code/PrivateMessage.cpp
    ../../../libstriezel/common/StringUtils.cpp
    ../../../libstriezel/filesystem/file.cpp
    ../../../libstriezel/hash/sha256/sha256.cpp
    ../../../libstriezel/hash/sha256/MessageSource.cpp
    ../../../libstriezel/zlib/CompressionFunctions.cpp
    save-load-compressed.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_definitions(-Wall -O3 -fexceptions)

    set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PM_save_load_compressed_test ${PM_save_load_compressed_test_src})

#find zlib
find_package (ZLIB)
if (ZLIB_FOUND)
  include_directories(${ZLIB_INCLUDE_DIRS})
  target_link_libraries (PM_save_load_compressed_test ${ZLIB_LIBRARIES})
else ()
  message ( FATAL_ERROR "zlib was not found!" )
endif (ZLIB_FOUND)

# --- add it as a test
add_test(NAME class_PrivateMessage_save_load_comp
         COMMAND $<TARGET_FILE:PM_save_load_compressed_test>)
