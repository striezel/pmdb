# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

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
add_test(NAME pmdb_no_comp_help_question_mark_win
         COMMAND $<TARGET_FILE:pmdb_no_comp> /?)

# parameter to show version number
add_test(NAME pmdb_no_comp_version
         COMMAND $<TARGET_FILE:pmdb_no_comp> --version)
