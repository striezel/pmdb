cmake_minimum_required (VERSION 3.8...3.31)

project(pmdb)

# allow tests
enable_testing()

# If the option ENABLE_LTO is enabled (e. g. via `cmake -DENABLE_LTO=ON`)
# during the build, then all binaries will use link-time optimization (LTO).
option(ENABLE_LTO "Enable link-time optimization" OFF)
# Not all compilers support LTO / IPO, so it has to be checked.
if (ENABLE_LTO)
  cmake_policy(SET CMP0069 NEW)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT HAS_LTO_SUPPORT OUTPUT LTO_FAIL_REASON
                      LANGUAGES C CXX)
  if (NOT HAS_LTO_SUPPORT)
    message(FATAL "IPO / LTO is not supported: ${LTO_FAIL_REASON}")
  else()
    message(STATUS "IPO / LTO is supported. Using it.")
  endif()
endif (ENABLE_LTO)

# If ENABLE_STATIC_STANDARD_LIBRARIES is on (e. g. via
# `cmake -DENABLE_STATIC_STANDARD_LIBRARIES=ON`), then the C++ standard
# libraries are linked statically. The option is off by default.
#
# Static linking increases the size of the binaries, but those binaries do not
# need the statically linked libraries to be present on the system.
option(ENABLE_STATIC_STANDARD_LIBRARIES "Link C++ standard libraries statically" OFF)
if (ENABLE_STATIC_STANDARD_LIBRARIES)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif (ENABLE_STATIC_STANDARD_LIBRARIES)

# If ENABLE_STATIC_LINKING is on (e. g. via `cmake -DENABLE_STATIC_LINKING=ON`),
# then all libraries are linked statically. The option is off by default.
#
# Static linking increases the size of the binaries, but those binaries do not
# need the libraries to be present on the system.
#
# WARNING: This option is still experimental and may not work completely.
option(ENABLE_STATIC_LINKING "Links all libraries statically" OFF)
if (ENABLE_STATIC_LINKING)
  set(CMAKE_LINK_SEARCH_START_STATIC 1)
  set(CMAKE_LINK_SEARCH_END_STATIC 1)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif ()

# option to enable sanitizers
option(ENABLE_SANITIZER "Enable sanitizer options" OFF)
if (ENABLE_SANITIZER)
  if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "Sanitizer builds need GCC or Clang compiler!")
  endif()
  message(STATUS "Enabling address sanitizer and the undefined behaviour sanitizer.")
  add_compile_options(-fsanitize=address -fno-omit-frame-pointer
                      -fsanitize=undefined -fno-sanitize-recover=all
                      -g)
  link_libraries(-fsanitize=address -fno-omit-frame-pointer
                 -fsanitize=undefined -fno-sanitize-recover=all)
endif (ENABLE_SANITIZER)

# option for code coverage
option(CODE_COVERAGE "Build with code coverage (GCC only)" OFF)
# If CODE_COVERAGE is on (e. g. via `cmake -DCODE_COVERAGE=ON`), then all
# binaries are built with code coverage. The option is off by default.
# Currently this only works with GCC.
if (CODE_COVERAGE)
  if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "Code coverage builds need GCC compiler!")
  endif ()
  # set coverage options for GCC
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
endif ()

#actual project
add_subdirectory (code)

# like actual project, but without compression support
add_subdirectory (program-no-compression)

# add subdirectory for test cases
add_subdirectory (tests)
