cmake_minimum_required(VERSION 3.12)

project(PhotoBatch)

add_executable(PhotoBatch "src/main.cpp" "src/ArgumentParser.cpp" "src/StringUtils.cpp" "src/Mode.h" "src/Mode.cpp"   "src/RenameMode.h" "src/RenameMode.cpp" "src/ConvertMode.h" "src/ConvertMode.cpp" "src/ResizeMode.h" "src/ResizeMode.cpp")

set_target_properties(PhotoBatch PROPERTIES CXX_STANDARD 17)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Desabilita o modo quiet do FetchContent" FORCE)
include(FetchContent)

add_subdirectory(external)

target_link_libraries(PhotoBatch PRIVATE stb)