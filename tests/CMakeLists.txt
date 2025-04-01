cmake_minimum_required (VERSION 3.8...3.31)

enable_testing()

include (CheckIncludeFileCXX)
check_include_file_cxx (catch2/catch_test_macros.hpp HAS_CATCH_V3_HEADER)
check_include_file_cxx (catch2/catch.hpp HAS_CATCH_V2_HEADER)

if (HAS_CATCH_V3_HEADER AND NOT HAS_CATCH_V2_HEADER)
  set(HAS_CATCH_V3 ON)
else ()
  set(HAS_CATCH_V3 OFF)
endif ()

add_subdirectory (components)

# Recurse into subdirectory for tests of ColourMap class.
add_subdirectory (colourmap)

# Recurse into subdirectory for tests of MessageDatabase class.
add_subdirectory (messagedatabase)

# Recurse into subdirectory for tests of MsgTemplate class.
add_subdirectory (msgtemplate)

# Recurse into subdirectory for tests of PrivateMessage class.
add_subdirectory (privatemessage)

# Recurse into subdirectory for tests of pmdb executable.
add_subdirectory (pmdb)

# Recurse into subdirectory for tests of pmdb executable without compression
# support.
add_subdirectory (pmdb-no-comp)
