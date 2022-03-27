add_library(install_package install.c install.h)
target_link_libraries(install_package -ljson-c)