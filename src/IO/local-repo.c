#include <json-c/json.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#include "json.h"
#include "../errors.h"
#include "../package.h" /* MAX_PACKAGE_NAME_SIZE */

void create_db(sqlite3 *db)
{
    printf("==> Creating local.db database...\n");
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
        printf("\033[31mSomething went wrong\n");
        sqlite3_close(db);
        exit(unkown_error);
    }

    char *sql = malloc(sizeof(char) * (strlen("DELETE FROM packages WHERE name = ''") + strlen(package_name) + strlen(";") + 1));

    strcpy(sql, "DELETE FROM packages WHERE name = '");
    strcat(sql, package_name);
    strcat(sql, "';");

    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);

    if (rc != SQLITE_OK) {
        printf("\033[31mSomething went wrong\n");
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        exit(unkown_error);
    }
}

void 
add_package_to_local_repo(package pkg, const char *package_name)
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
    const char *package_version = json_object_get_string(pkg.version);
    const char *package_description = json_object_get_string(pkg.description);

    /* 
    * Dependencies, removal and update instructions (which are all arrays) would be saved differently in the database 
    * Because they are arrays we would separate their values between semicolons
    * In this way dependencies would be easily read and commands can also be executed 
    */

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

    //printf("\n\n%s\n\n", sql);

    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);

    if (rc)
    {
        printf("\033[31mUnable to add package to local repo (%s)\n", zErrMsg);
        exit(unkown_error);
    }

    free(sql);
}


int 
callback(void *ptr, int column_num, char **values, char **rows)
{
    // We need to write to a temporary file the values given to us by sqlite3
    // The values would be separated by a newline and then be read with fscanf

    char *temp_file = "/tmp/japm_sql_callback_temp_file";
    //We empty the file
    system("echo '' > /tmp/japm_sql_callback_temp_file");

    FILE *fp = fopen(temp_file, "a");

    if (fp == NULL)
    {
        printf("Unable to open file\n");
        exit(unkown_error);
    }

    for (int i = 0; i < column_num; i++)
    {
        fprintf(fp, "%s\n", values[i]);
    }

    fclose(fp);
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
        (strlen("SELECT * FROM packages WHERE name = ''") 
        + strlen(package_name) + strlen(";") + 1));

    strcpy(sql, "SELECT * FROM packages WHERE name = '");

    strcat(sql, package_name);
    strcat(sql, "';");

    printf("\n\n%s\n\n", sql);

    rc = sqlite3_exec(db, sql, callback, NULL, &zErrMsg);

    if (rc)
    {
        //Print in red the error message
        printf("\033[0;31mSomething went wrong (SQL error: %s)\033[0m\n", zErrMsg);
        exit(unkown_error);
    }

    //We read the values from the file

    FILE *fp = fopen("/tmp/japm_sql_callback_temp_file", "r");

    if (fp == NULL)
    {
        printf("\033[31mUnable to open temporary file, aborting...\n");
        exit(unkown_error);
    }

    package return_package;
    return_package.name = json_object_new_object();
    return_package.version = json_object_new_object();
    return_package.description = json_object_new_object();
    return_package.dependencies = json_object_new_object();
    return_package.remove = json_object_new_object();
    return_package.update = json_object_new_object();

    json_object *json_fields_to_be_read[6] = {
        return_package.name,
        return_package.version,
        return_package.description,
        return_package.dependencies,
        return_package.remove,
        return_package.update
    };

    for (int i = 0; i < 6; i++)
    {
        char *line = malloc(sizeof(fp)); //Not the most efficient thing but it does the job
        fscanf(fp, "%s", line);
        printf("%s\n", line);
        json_object_set_string(json_fields_to_be_read[i], line);
        free(line);
    }

    return return_package;

    free(sql);
}

void 
update_package_in_local_repository(package pkg)
{
    // We open the /var/japm/repos/local.db file and update the package using sqlite3
    start_again: ;
    
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("/var/japm/repos/local.db", &db);

    if (rc) {
        create_db(db); 
        goto start_again;
    }

    // We update the package in the database

    char *sql = malloc(sizeof(char) * 
        (strlen("UPDATE packages SET version = ") 
        + strlen(json_object_get_string(pkg.version)) + strlen(", description = ") 
        + strlen(json_object_get_string(pkg.description)) + strlen(", dependencies = ") 
        + strlen(json_object_get_string(pkg.dependencies)) + strlen(", remove_instructions = ") 
        + strlen(json_object_get_string(pkg.remove)) + strlen(", update_instructions = ") 
        + strlen(json_object_get_string(pkg.update)) + strlen(" WHERE name = ") 
        + strlen(json_object_get_string(pkg.name)) + strlen(";") + 1));

    strcpy(sql, "UPDATE packages SET version = ");

    strcat(sql, json_object_get_string(pkg.version));
    strcat(sql, ", description = ");
    strcat(sql, json_object_get_string(pkg.description));
    strcat(sql, ", dependencies = ");
    strcat(sql, json_object_get_string(pkg.dependencies));
    strcat(sql, ", remove_instructions = ");
    strcat(sql, json_object_get_string(pkg.remove));
    strcat(sql, ", update_instructions = ");
    strcat(sql, json_object_get_string(pkg.update));
    strcat(sql, " WHERE name = ");
    strcat(sql, json_object_get_string(pkg.name));
    strcat(sql, ";");

    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);

    if (rc)
    {
        //Print in red the error message
        printf("\033[0;31mSomething went wrong (SQL error: %s)\033[0m\n", zErrMsg);
        exit(unkown_error);
    }

    free(sql);
    sqlite3_close(db);
}
