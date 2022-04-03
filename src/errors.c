#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define ACCEPTED_ARGS_LENGTH 4

char *accepted_args[ACCEPTED_ARGS_LENGTH] = {"install", "remove", "update", "search"};

/* USAGE CHECKING */

void usage(char *program_name)
{
    printf("\033[0mUsage:\n");
    printf("    %s [OPTIONS]\n", program_name);
    printf("       install     -> Install package\n");
    printf("       remove      -> Remove package\n");
    printf("       update      -> Update package/all packages\n");
    printf("       search      -> Search a package\n");
}

void bad_usage(char *arg, char *program_name)
{
    printf("\033[31mWrong Argument \"%s\"\n\n", arg);
    usage(program_name);
}

int check_usage(int argc, char **argv)
{
    if (argc == 1)
    {
        usage(argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        for (int j = 0; j < ACCEPTED_ARGS_LENGTH; j++)
        {
            if (strcmp(argv[i], accepted_args[j]) == 0)
            {
                return 0;
            }
        }

        bad_usage(argv[i], argv[0]);
        return 1;
    }

    return 0;
}

void install_usage(char *program_name)
{
    printf("\033[0mUsage:\n");
    printf("    %s install [package]\n", program_name);
}

int check_install_usage(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("\033[31mBad Usage\n\n");
        install_usage(argv[0]);
        return 1;
    }

    return 0;
}

void remove_usage(char *program_name)
{
    printf("\033[0mUsage:\n");
    printf("    %s remove [options] [package]\n", program_name);
    printf("              r    -> Remove package and the package that depend on it\n");
}

int check_remove_usage(int argc, char **argv)
{
    // Return 0 if removing a single package
    // Return 1 if bad usage
    // Return 2 if removing all packages that depend on the package alongside with the package itself

    if (argc < 3)
    {
        printf("\033[31mBad Usage\n\n");
        remove_usage(argv[0]);
        return 1;
    }
    else if (argc == 4)
    {
        if (strcmp(argv[2], "r") != 0)
        {
            bad_usage(argv[2], argv[0]);
            remove_usage(argv[0]);
            return 1;
        }
        else
        {
            return 2;
        }
    }

    return 0;
}

/* SUDO AND ROOT PREVILIGIES CHECKING */

int check_root()
{
    if (geteuid() != 0)
    {
        printf("\033[31mProgram needs to be run with root previliegies\n");
        return 1;
    }

    return 0;
}
