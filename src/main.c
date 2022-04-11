#include <stdio.h>
#include <string.h>

#include "errors.h"
#include "install.h"
#include "main.h"
#include "remove.h"
#include "update.h"
#include "IO/term.h"

int main(int argc, char **argv)
{
    if (check_root())
        return 2;

    if (check_usage(argc, argv))
        return 1;

    int input_type = check_input(argc, argv);

    switch (input_type)
    {
    case 1:
		//TODO: Multiple package install
        install_package(argc, argv);
		break;
    case 2:
		//TODO: Multiple package removal
        remove_package(argv[2]);
		break;
	case 3:
		//TODO: Multiple package updates
		if (strcmp(argv[2], "system") == 0) update_system();
		else update_package(argv[2]);
    }

    return 0;
}

int check_input(int argc, char **argv)
{
    if (strcmp(argv[1], "install") == 0)
        return 1;
    if (strcmp(argv[1], "remove") == 0)
        return 2;
    if (strcmp(argv[1], "update") == 0)
        return 3;
    if (strcmp(argv[1], "upgrade") == 0)
        return 4;
}
