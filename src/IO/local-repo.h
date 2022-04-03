#include "json.h"

void add_package_to_local_repo(package pkg, char *package_name);
package get_package_from_local_repo(char* package_name);
void remove_package_from_local_repository(char* package_name);
void update_package_in_local_repository(package pkg);
