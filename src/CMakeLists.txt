cmake_minimum_required(VERSION 3.22.0)

project(japm)

add_library(errors errors.c errors.h)
add_library(IO IO/json.c IO/json.h)
add_library(install_pkg install.c install.h)
add_library(package package.c package.h)
add_library(internet internet.c internet.h)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} errors install_pkg package IO internet)

target_link_libraries(${PROJECT_NAME} json-c)
