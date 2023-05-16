#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "japml.h"
#include "package.h"
#include "db.h"
#include "list.h"
#include "file.h"
#include "helper.h"

japml_package_t* japml_package_create_empty()
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

    char* package_used_by_file = malloc(strlen(dir) + strlen(pkg->name) + 1);
    sprintf(package_used_by_file, "%s%s", dir, pkg->name);

    if(access(package_used_by_file, F_OK) != 0)
    {
        japml_file_create_recursive(package_used_by_file);
    }

    free(dir);
    return package_used_by_file;
}

void japml_package_mark_dependencies(japml_handle_t* handle, japml_package_t* package)
{
    japml_list_t* dep = package->deps;
    while (dep)
    {
        japml_package_append_depender(handle, (char*)(dep->data), package->name);
        dep = japml_list_next(dep);
    }
}

void japml_package_append_depender(japml_handle_t* handle, char* pkg, char* depender)
{
    japml_package_t* temp_pkg = japml_package_create_empty();
    
    // Copying since assinging directly will free pkg and cause further errors
    temp_pkg->name = malloc(strlen(pkg) + 1);
    strcpy(temp_pkg->name, pkg);

    char* file = japml_get_used_by_file(temp_pkg);

    japml_package_free(temp_pkg);

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

void japml_package_remove_depender(japml_package_t* depender)
{
    japml_list_t* it = depender->deps;

    while (it)
    {
        japml_package_t* package = (japml_package_t*)(it->data);
        char* file = japml_get_used_by_file(package);

        FILE *f = fopen(file, "r");
        
        japml_file_create_recursive(PACKAGE_USED_BY_TMP);
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

        japml_file_copy(PACKAGE_USED_BY_TMP, file);

        it = japml_list_next(it);
    }
}

void japml_package_get_depending(japml_handle_t* handle, japml_package_t* package)
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
        japml_file_create_recursive(file);
        goto restart_open_used_by;
    }

    char chunk[MAX_PACKAGE_NAME_LENGTH];
    package->depending_packages = NULL;
    while(fgets(chunk, sizeof(chunk), f) != NULL) {
        chunk[strlen(chunk) - 1] = '\0';
        japml_package_t* pkg = japml_db_local_get_package(handle, chunk);
        japml_list_add(&package->depending_packages, pkg);
    }

    fclose(f);
    free(file);
}

int japml_package_add_to_list_no_rep(japml_list_t** list, japml_package_t* package)
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

    japml_list_add(list, package);
    return 0;
}

/* Frees a package file */
void japml_package_free_f(void* ptr)
{
    japml_package_file_t* file = (japml_package_file_t*)ptr;
    free(file->url);
    free(file->rel_file_loc);
    free(file);
}

void japml_package_free(japml_package_t* package)
{
    if (package == NULL)
    {
        return;
    }

    free(package->name);
    free(package->version);
    free(package->description);

    // ! Imp. deps are referenced by string not by a package strucutre
    japml_list_free_data(package->build_deps);
    japml_list_free_data(package->deps);

    japml_package_free_list(package->depending_packages);

    japml_list_free_data(package->pre_install);
    japml_list_free_data(package->install);

    japml_list_free_recursive(package->files, japml_package_free_f);

    japml_list_free_data(package->post_install);
    japml_list_free_data(package->remove);

    free(package);
}

void japml_package_free_list(japml_list_t* packages)
{
    japml_list_t* it = packages;
    while (it)
    {
        japml_package_free((japml_package_t*)(it->data));
        it = japml_list_next(it);
    }

    japml_list_free(packages);
}

void japml_package_print_list(japml_handle_t* handle, japml_list_t* packages)
{
    while (packages)
    {
        sprintf(handle->log_message, "  %s", ((japml_package_t*)packages->data)->name);
        japml_log(handle, Information, handle->log_message);
        packages = japml_list_next(packages);
    }
}

void japml_package_print_details(japml_handle_t* handle, japml_package_t* package)
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