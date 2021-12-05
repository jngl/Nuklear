cmake_minimum_required(VERSION 3.16)
project(NuklearCpp)

file(GLOB_RECURSE nuklear_source_files src/*)
add_library(nuklear ${nuklear_source_files})
target_include_directories(nuklear PUBLIC src)

add_subdirectory(example)
