# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

enable_testing()

# MSYS2 / MinGW uses Catch 3.x.
if (MINGW)
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
