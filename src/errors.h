/* Usage */

void usage(char*program_name);
void bad_usage(char*arg, char*program_name); //Arg is the wrong argument that gave us the error
int check_usage(int argc, char**argv);

void install_usage(char*program_name);
int check_install_usage(int argc, char**argv);


void remove_usage(char*program_name);
int check_remove_usage(int argc, char**argv);

/* sudo and root previliegies */

int check_root();

enum error_codes {
    wrong_usage_error = 1,
    package_not_found_error = 2,
    package_corrupted_error = 3,
    package_already_installed_error = 4,
    dependency_break_error = 5,
    unkown_error = 10
};
