#include <json-c/json.h>
#include <sqlite3.h>
#include <string.h>

#include "json.h"
#include "../errors.h"
#include "../package.h" /* MAX_PACKAGE_NAME_SIZE */

void create_db(sqlite3 *db)
{
    printf("Creating local.db database...\n");
    int rc;
    rc = 0;

    system("mkdir -p /var/japm/repos");
    system("touch /var/japm/repos/local.db");
    
    rc = sqlite3_open("/var/japm/repos/local.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(unkown_error);
    }

    //We create the table

    sqlite3_exec(db,
    "CREATE TABLE packages (\"name\" TEXT PRIMARY KEY NOT NULL, \"version\" TEXT NOT NULL, \"description\" TEXT NOT NULL, \"dependencies\" TEXT NOT NULL, \"remove_instructions\" TEXT NOT NULL, update_instructions TEXT NOT NULL);",
    NULL, NULL, NULL);
}

void 
remove_package_from_local_repository(char* package_name)
{
    sqlite3 *db;
    int rc = 0;
    char *zErrMsg = 0;

    rc = sqlite3_open("/var/japm/repos/local.db", &db);

    if (rc) {
        printf("Something went wrong\n");
        sqlite3_close(db);
        exit(unkown_error);
    }

    char *sql = malloc(sizeof(char) * (strlen("DELETE FROM packages WHERE name = ''") + strlen(package_name) + strlen(";") + 1));

    strcpy(sql, "DELETE FROM packages WHERE name = '");
    strcat(sql, package_name);
    strcat(sql, "';");

    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);

    if (rc != SQLITE_OK) {
        printf("Something went wrong\n");
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        exit(unkown_error);
    }
}

void 
add_package_to_local_repo(package pkg)
{
    //TODO: Check for sql injection
    
    start_again: ;
    // We open the /var/japm/repos/local.db file and add the package using sqlite3

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("/var/japm/repos/local.db", &db);

    if (rc) {
        create_db(db);
        goto start_again;
    }
    
    //We add the package to the database
    const char *package_name = json_object_get_string(pkg.name);
    const char *package_version = json_object_get_string(pkg.version);
    const char *package_description = json_object_get_string(pkg.description);

    char *package_dependencies = calloc(strlen(json_object_get_string(pkg.dependencies)), sizeof(char));
    for (int i = 0; i < json_object_array_length(pkg.dependencies); i++)
    {
        strcat(package_dependencies, json_object_get_string(json_object_array_get_idx(pkg.dependencies, i)));
        strcat(package_dependencies, ";");
    }
    
    char *package_remove = calloc(strlen(json_object_get_string(pkg.remove)), sizeof(char));
    for (int i = 0; i < json_object_array_length(pkg.remove); i++)
    {
        strcat(package_remove, json_object_get_string(json_object_array_get_idx(pkg.remove, i)));
        strcat(package_remove, ";");
    }

    char *package_update = calloc(strlen(json_object_get_string(pkg.update)), sizeof(char));
    for (int i = 0; i < json_object_array_length(pkg.update); i++)
    {
        strcat(package_update, json_object_get_string(json_object_array_get_idx(pkg.update, i)));
        strcat(package_update, ";");
    }
    
    char *sql = malloc(sizeof(char) * 
        (strlen("INSERT INTO packages (name, version, description, dependencies, remove_instructions, update_instructions) VALUES (") 
        + strlen(package_name) + strlen("'', ") 
        + strlen(package_version) + strlen("'', ") 
        + strlen(package_description) + strlen("'', ") 
        + strlen(package_dependencies) + strlen("'', ") 
        + strlen(package_remove) + strlen("'', ")
        + strlen(package_update) + strlen("'');")) + 1);

    strcpy(sql, "INSERT INTO packages (name, version, description, dependencies, remove_instructions, update_instructions) VALUES (");
    
    strcat(sql, "'");
    strcat(sql, package_name);
    strcat(sql, "', ");
   
    strcat(sql, "'");
    strcat(sql, package_version);
    strcat(sql, "', ");
    
    strcat(sql, "'");
    strcat(sql, package_description);
    strcat(sql, "', ");

    strcat(sql, "'");
    strcat(sql, package_dependencies);
    strcat(sql, "', ");
    
    strcat(sql, "'");
    strcat(sql, package_remove);
    strcat(sql, "', ");

    strcat(sql, "'");
    strcat(sql, package_update);
    strcat(sql, "');");

    printf("\n\n%s\n\n", sql);

    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);

    if (rc)
    {
        printf("Unable to add package to local repo %s\n", zErrMsg);
        exit(unkown_error);
    }

    free(sql);
}

package return_pkg;

int 
callback(void *ptr, int column_num, char **values, char **rows)
{
    system("touch /tmp/japm_database_read_package_info");

    FILE *f = fopen("/tmp/japm_database_read_package_info", "w");

    if (f == NULL)
    {
        exit(unkown_error);
    }

    fprintf(f, "%s %s %s %s", values[0]);

    json_object_set_string(return_pkg.name, values[0]);
    json_object_set_string(return_pkg.version, values[1]);
    json_object_set_string(return_pkg.description, values[2]);
    
    //TODO: Separate instructions between commas 
    json_object_set_string(return_pkg.dependencies, values[3]);
    json_object_set_string(return_pkg.remove, values[4]);
    json_object_set_string(return_pkg.update, values[5]);

    return 0;
}

package
get_package_from_local_repo(char* package_name)
{
    start_again: ;
    // We open the /var/japm/repos/local.db file and get the package using sqlite3

    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("/var/japm/repos/local.db", &db);

    if (rc) {
        create_db(db); 
        goto start_again;
    }

    //We get the package from the database

    char *sql = malloc(sizeof(char) * 
        (strlen("SELECT * FROM packages WHERE name = ") 
        + strlen(package_name) + strlen(";") + 1));

    strcpy(sql, "SELECT * FROM packages WHERE name = ");

    strcat(sql, package_name);
    strcat(sql, ";");

    rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);

    free(sql);

    return return_pkg;
}
