cmake_minimum_required(VERSION 3.12)

project(PhotoBatch)

add_executable(PhotoBatch "src/main.cpp" "src/ArgumentParser.cpp" "src/StringUtils.cpp" "src/Mode.h" "src/Mode.cpp"   "src/RenameMode.h" "src/RenameMode.cpp")

set_target_properties(PhotoBatch PROPERTIES CXX_STANDARD 17)