cmake_minimum_required(VERSION 3.16)

add_library(acrylic 
    acrylic.c
    synth/filter.c
    synth/envelope.c
    synth/oscillator.c
    synth/util.c)

target_link_libraries(acrylic PRIVATE lvgl)
target_include_directories(acrylic PUBLIC .)
