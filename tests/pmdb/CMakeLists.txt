# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# add tests for --version and --help parameters

add_test(NAME pmdb_help
         COMMAND $<TARGET_FILE:pmdb> --help)

add_test(NAME pmdb_version
         COMMAND $<TARGET_FILE:pmdb> --version)
