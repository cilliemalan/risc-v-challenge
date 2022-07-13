cmake_minimum_required(VERSION 3.16)

project(synth)

set(LV_CONF_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/lv_conf.h" CACHE STRING "lvgl conf path" FORCE)

add_executable(main
    src/main.c)

add_subdirectory(raylib)
add_subdirectory(lvgl)
add_subdirectory(src/acrylic)

target_link_libraries(main PRIVATE raylib lvgl acrylic)
