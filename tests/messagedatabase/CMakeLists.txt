cmake_minimum_required (VERSION 3.8...3.31)

# Recurse into subdirectory for importFromFile() test.
add_subdirectory (importFromFile)

# Recurse into subdirectory for saveMessages()/loadMessages() test with plain
# text (i.e. uncompressed) files.
add_subdirectory (save-load)

# Recurse into subdirectory for saveMessages()/loadMessages() test with
# compressed files.
add_subdirectory (save-load-compressed)
