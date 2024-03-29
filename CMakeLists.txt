cmake_minimum_required(VERSION 3.18.0)

project(japm)

include_directories(src lib/libjapml)

include_directories(lib)

add_library(
    japml 
    lib/libjapml/action.c
    lib/libjapml/action.h
    lib/libjapml/colors.h
    lib/libjapml/db.c
    lib/libjapml/db.h
    lib/libjapml/error.c
    lib/libjapml/error.h
    lib/libjapml/exit.c
    lib/libjapml/exit.h
    lib/libjapml/file.h
    lib/libjapml/file.c
    lib/libjapml/handle.h
    lib/libjapml/helper.c
    lib/libjapml/helper.h
    lib/libjapml/init.c
    lib/libjapml/init.h
    lib/libjapml/input.h
    lib/libjapml/install.c
    lib/libjapml/install.h
    lib/libjapml/internet.c
    lib/libjapml/internet.h
    lib/libjapml/japmlcurses.c
    lib/libjapml/japmlcurses.h
    lib/libjapml/japml.h
    lib/libjapml/json.c
    lib/libjapml/json.h
    lib/libjapml/list.c
    lib/libjapml/list.h
    lib/libjapml/log.c
    lib/libjapml/log.h
    lib/libjapml/package.c
    lib/libjapml/package.h
    lib/libjapml/parser.c
    lib/libjapml/parser.h
    lib/libjapml/remove.c
    lib/libjapml/remove.h
    lib/libjapml/system.c
    lib/libjapml/system.h
    )

add_executable(${PROJECT_NAME} 
    src/main.c
    src/devel.h
    src/devel.c
    src/install.h
    src/install.c
    src/remove.h
    src/remove.c
    src/update.h
    src/update.c
    src/search.h
    src/search.c
    )

target_link_libraries(${PROJECT_NAME} ncurses japml curl sqlite3 json-c)
