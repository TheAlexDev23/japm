cmake_minimum_required(VERSION 3.22.0)

project(japm)

add_executable(${PROJECT_NAME} main.c errors.c install.c)
target_link_libraries(${PROJECT_NAME} json-c)
