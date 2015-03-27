# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# binary for Config creation test
project(config_test_create)

set(config_test_create_src
    ../../code/Config.cpp
    ../../code/bbcode/Smilie.cpp
    config_test_create.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(config_test_create ${config_test_create_src})

# --- add it as a test
add_test(NAME class_Config_create
         COMMAND $<TARGET_FILE:config_test_create>)


# binary for Config set URL test
project(config_test_setURL)

set(config_test_setURL_src
    ../../code/Config.cpp
    ../../code/bbcode/Smilie.cpp
    config_test_setURL.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(config_test_setURL ${config_test_setURL_src})

# --- add it as a test
add_test(NAME class_Config_setURL
         COMMAND $<TARGET_FILE:config_test_setURL>)



# binary for Config set TPL test
project(config_test_setTPL)

set(config_test_setTPL_src
    ../../code/Config.cpp
    ../../code/bbcode/Smilie.cpp
    config_test_setTPL.cpp)

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-Wall -O3 -fexceptions)
endif (CMAKE_COMPILER_IS_GNUCC)

set( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -s" )

add_executable(config_test_setTPL ${config_test_setTPL_src})

# --- add it as a test
add_test(NAME class_Config_setTPL
         COMMAND $<TARGET_FILE:config_test_setTPL>)
