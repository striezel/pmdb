# We might support earlier versions, too, but it's only tested with 2.8.9.
cmake_minimum_required (VERSION 2.8)

# Recurse into subdirectory for addMessage() test.
add_subdirectory (addMessage)

# Recurse into subdirectory for importFromFile() test.
add_subdirectory (importFromFile)

# Recurse into subdirectory for saveMessages()/loadMessages() test.
add_subdirectory (save-load)
