cmake_minimum_required(VERSION 3.18.0)

project(japm)

include_directories(src lib/libjapml)

include_directories(lib)

add_library(
    japml 
    lib/libjapml/japml.h
    lib/libjapml/colors.h
    lib/libjapml/db.h
    lib/libjapml/error.c
    lib/libjapml/error.h
    lib/libjapml/exit.c
    lib/libjapml/exit.h
    lib/libjapml/handle.h
    lib/libjapml/japmlcurses.c
    lib/libjapml/japmlcurses.h
    lib/libjapml/list.c
    lib/libjapml/list.h
    lib/libjapml/log.c
    lib/libjapml/log.h
    lib/libjapml/package.h
    )

add_executable(${PROJECT_NAME} src/main.c)

target_link_libraries(${PROJECT_NAME} ncurses japml)
