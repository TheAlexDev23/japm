cmake_minimum_required(VERSION 3.18.0)

project(japm)

add_library(errors errors.c errors.h)
add_library(IO IO/json.c IO/json.h)
add_library(install_pkg install.c install.h)
add_library(package package.c package.h)
add_library(internet internet.c internet.h)
add_library(remove_pkg remove.c remove.h)
add_library(local_repo IO/local-repo.c IO/local-repo.h)
add_library(term IO/term.c IO/term.h)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} errors install_pkg package IO internet remove_pkg local_repo term)

target_link_libraries(${PROJECT_NAME} json-c sqlite3)
