#include "../Errors/errors.h"

int
install_package(int argc, char**argv) 
{
    if (check_install_usage(argc, argv)) 
    {
        return 1;
    }
    printf("Package downloading\n");
    return 0;
}