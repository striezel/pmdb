cmake_minimum_required (VERSION 3.8...3.31)

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
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
  add_test(NAME pmdb_help_question_mark_win
           COMMAND $<TARGET_FILE:pmdb> /?)
endif ()

# parameter to show version number
add_test(NAME pmdb_version
         COMMAND $<TARGET_FILE:pmdb> --version)
