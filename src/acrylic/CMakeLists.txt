cmake_minimum_required(VERSION 3.16)

add_library(acrylic 
    acrylic.c
    synth/envelope.c
    synth/filter.c
    synth/limiter.c
    synth/oscillator.c
    synth/synth.c
    synth/util.c)

target_link_libraries(acrylic PRIVATE lvgl)
target_include_directories(acrylic PUBLIC .)
