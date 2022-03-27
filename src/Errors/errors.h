/* Usage */

void usage(char*program_name);
void bad_usage(char*arg, char*program_name); //Arg is the wrong argument that gave us the error
int check_usage(int argc, char**argv);

void install_usage(char*program_name);
int check_install_usage(int argc, char**argv);

/* sudo and root previliegies */

int check_root();
