# We might support earlier versions, too, but try to use a recent version.
cmake_minimum_required (VERSION 3.8)

# *************************************
# * tests for default version of pmdb *
# *************************************

# add tests for --version and --help parameters
# default help parameter "--help"
add_test(NAME pmdb_help
         COMMAND $<TARGET_FILE:pmdb> --help)

# short help parameter with question mark "-?"
add_test(NAME pmdb_help_question_mark
         COMMAND $<TARGET_FILE:pmdb> -?)

# Windows-style help parameter "/?"
add_test(NAME pmdb_help_question_mark_win
         COMMAND $<TARGET_FILE:pmdb> /?)

# parameter to show version number
add_test(NAME pmdb_version
         COMMAND $<TARGET_FILE:pmdb> --version)
