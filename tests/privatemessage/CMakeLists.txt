cmake_minimum_required (VERSION 3.8)

# Recurse into subdirectory for normalise() test.
add_subdirectory (normalise)

# Recurse into subdirectory for saveToFile() + loadFromFile() test without
# compression during loading/saving data.
add_subdirectory (save-load)

# Recurse into subdirectory for saveToFile() + loadFromFile() test with
# compressed data.
add_subdirectory (save-load-compressed)
