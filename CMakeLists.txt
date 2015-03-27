# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# allow tests
enable_testing()

#actual project
add_subdirectory (code)

# add subdirectory for test cases
add_subdirectory (tests)
