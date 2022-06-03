# We might support earlier versions, too, but try to use a recent one.
cmake_minimum_required (VERSION 3.8)

project(pmdb)

# allow tests
enable_testing()

#actual project
add_subdirectory (code)

# like actual project, but without compression support
add_subdirectory (program-no-compression)

# add subdirectory for test cases
add_subdirectory (tests)
