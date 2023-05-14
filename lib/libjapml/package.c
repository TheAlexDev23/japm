#include <stdio.h>
#include <string.h>

#include "japml.h"
#include "package.h"
#include "db.h"
#include "list.h"
#include "file.h"

japml_package_t* japml_create_empty_package()
{
    japml_package_t* package = malloc(sizeof(japml_package_t));

    package->name =
    package->description =
    package->version = NULL;

    package->build_deps =
    package->deps =
    package->depending_packages =
    package->pre_install =
    package->install = 
    package->files =
    package->post_install =
    package->remove = NULL;

    return package;
}

char* japml_get_used_by_file(japml_package_t* pkg)
{
    char* dir = japml_get_package_directory(pkg);

    char* package_used_by_file = malloc(strlen(dir) + strlen(pkg->name));
    sprintf(package_used_by_file, "%s%s", dir, pkg->name);
    free(dir);
    return package_used_by_file;
}

void japml_append_depenending_package(japml_handle_t* handle, char* pkg, char* depender)
{
    japml_package_t* temp_pkg = japml_create_empty_package();
    temp_pkg->name = pkg;
    char* file = japml_get_used_by_file(temp_pkg);
    japml_free_package(temp_pkg);

    FILE *f = fopen(file, "r");
    if (f == NULL)
    {
        japml_throw_error(handle, custom_error_critical, "Could not open used_by file");
    }

    // Esentially if depender->name is found already don't add to list
    char pkg_name[MAX_PACKAGE_NAME_LENGTH];
    while (fscanf(f, "%s", pkg_name) != EOF)
    {
        if (strcmp(pkg_name, depender) == 0)
        {
            return;
        }
    }

    fclose(f);
    f = fopen(file, "a");

    fprintf(f, "%s\n", depender);

    free(file);
    fclose(f);
}

void japml_remove_depending_package(japml_handle_t* handle, japml_package_t* depender)
{
    japml_list_t* it = depender->deps;

    while (it)
    {
        japml_package_t* package = (japml_package_t*)(it->data);
        char* file = japml_get_used_by_file(package);

        FILE *f = fopen(file, "r");
        
        japml_create_file_recursive(PACKAGE_USED_BY_TMP);
        FILE *tmp = fopen(PACKAGE_USED_BY_TMP, "w");

        // Essentially copy into tmp all packages except depender->name
        char chunk[MAX_PACKAGE_NAME_LENGTH];
        while(fgets(chunk, sizeof(chunk), f)) 
        {
            chunk[strlen(chunk) - 1] = '\0';
            if (strcmp(chunk, depender->name) != 0)
            {
                fprintf(tmp, "%s\n", chunk);
            }
        }

        fclose(tmp);
        fclose(f);

        japml_copy_file(PACKAGE_USED_BY_TMP, file);

        it = japml_list_next(it);
    }
}

void japml_get_depending_packages(japml_handle_t* handle, japml_package_t* package)
{
    if (!package)
    {
        return;
    }

    char* file = japml_get_used_by_file(package);

    int i = -1;
    restart_open_used_by: ;
    i++;
    FILE *f = fopen(file, "r");

    if (!f && !i)
    {
        japml_create_file_recursive(file);
        goto restart_open_used_by;
    }

    char chunk[MAX_PACKAGE_NAME_LENGTH];
    package->depending_packages = NULL;
    while(fgets(chunk, sizeof(chunk), f) != NULL) {
        chunk[strlen(chunk) - 1] = '\0';
        japml_package_t* pkg = japml_get_package_from_local_db(handle, chunk);
        japml_list_add(handle, &package->depending_packages, pkg);
    }

    free(file);
}

int japml_add_package_to_list_no_repeat(japml_handle_t* handle, japml_list_t** list, japml_package_t* package)
{
    japml_list_t* it = *list;
    while (it)
    {
        if (strcmp(((japml_package_t*)(it->data))->name, package->name) == 0)
        {
            return 1;
        }

        it = japml_list_next(it);
    }

    japml_list_add(handle, list, package);
    return 0;
}

void japml_free_package(japml_package_t* package)
{
    if (package == NULL)
    {
        return;
    }

    japml_list_free(package->build_deps);
    japml_list_free(package->deps);
    japml_list_free(package->depending_packages);
    japml_list_free(package->pre_install);
    japml_list_free(package->install);
    japml_list_free(package->files);
    japml_list_free(package->post_install);
    japml_list_free(package->remove);

    free(package);
}

void japml_free_package_list(japml_list_t* packages)
{
    while (packages)
    {
        japml_free_package((japml_package_t*)(packages->data));
        packages = japml_list_next(packages);
    }

    japml_list_free(packages);
}

void japml_print_package_list(japml_handle_t* handle, japml_list_t* packages)
{
    while (packages)
    {
        sprintf(handle->log_message, "  %s", ((japml_package_t*)packages->data)->name);
        japml_log(handle, Information, handle->log_message);
        packages = japml_list_next(packages);
    }
}

void japml_print_package_details(japml_handle_t* handle, japml_package_t* package)
{
    if (package == NULL)
    {
        return;
    }

    sprintf(handle->log_message, "Name: %s", package->name);
    japml_log(handle, Information, handle->log_message);

    sprintf(handle->log_message, "Version: %s", package->version);
    japml_log(handle, Information, handle->log_message);

    sprintf(handle->log_message, "Description: %s", package->description);
    japml_log(handle, Information, handle->log_message);
}