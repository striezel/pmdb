cmake_minimum_required (VERSION 3.8)

# ********************************************
# * tests for "no compression" build of pmdb *
# ********************************************

# add tests for --version and --help parameters
# default help parameter "--help"
add_test(NAME pmdb_no_comp_help
         COMMAND $<TARGET_FILE:pmdb_no_comp> --help)

# short help parameter with question mark "-?"
add_test(NAME pmdb_no_comp_help_question_mark
         COMMAND $<TARGET_FILE:pmdb_no_comp> -?)

# Windows-style help parameter "/?"
if (NOT DEFINED ENV{GITHUB_ACTIONS} OR NOT MINGW)
  add_test(NAME pmdb_no_comp_help_question_mark_win
           COMMAND $<TARGET_FILE:pmdb_no_comp> /?)
endif ()

# parameter to show version number
add_test(NAME pmdb_no_comp_version
         COMMAND $<TARGET_FILE:pmdb_no_comp> --version)
