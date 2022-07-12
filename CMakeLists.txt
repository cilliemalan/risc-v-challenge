cmake_minimum_required(VERSION 3.16)

project(synth)

add_executable(main
    src/main.c)

add_subdirectory(raylib)

target_link_libraries(main PRIVATE raylib)
