cmake_minimum_required (VERSION 3.12)

project(FinalProject-041024402)

add_executable(FinalProject-041024402 "FinalProject-041024402.cpp")

target_include_directories(FinalProject-041024402 PUBLIC "./include")
target_link_directories(FinalProject-041024402 PUBLIC "./lib")
target_link_libraries(FinalProject-041024402 sfml-main-d sfml-audio-d sfml-graphics-d sfml-system-d sfml-window-d tgui-d)