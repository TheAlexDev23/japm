cmake_minimum_required(VERSION 3.18.0)

project(japm)

add_executable(${PROJECT_NAME} 
main.c
../lib/libjapml/error.c
../lib/libjapml/exit.c
../lib/libjapml/japmlcurses.c
../lib/libjapml/list.c
../lib/libjapml/log.c
)

target_link_libraries(${PROJECT_NAME} ncurses)
