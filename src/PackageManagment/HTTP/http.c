#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../Errors/errors.h"

int http_req(char*url) 
{
    /* Create a HTTP request to the provided argument url and return the response code */
    /* To do the request I will call curl command using the system function */
    /* The response code would be saved in a temporary file that would then be read */
    /* The response code would be returned */
    char *command = malloc(sizeof(char) * (strlen("curl -s -o /dev/null -w %{http_code} ") + strlen(url) + strlen(" > /tmp/reponse_code.txt") + 1));
    strcpy(command, "curl -s -o /dev/null -w %{http_code} ");
    strcat(command, url);
    strcat(command, " > /tmp/response_code.txt");
    system(command);

    FILE *fp = fopen("/tmp/response_code.txt", "r");
    if (fp == NULL)
    {
        printf("Something went wrong. Check your internet connection\n");
        exit(unkown_error);
    }

    // Read the response code from the file
    char response_code[4];
    fscanf(fp, "%s", response_code);
    fclose(fp);
    free(command);

    //Convert the reponse code to integer and return it
    return atoi(response_code);
}

void 
download_package(char*url, char*package_name)
{
    // Download the package file from the provided url
    // The file would be saved in /var/cache/japm/package_name
    
    system("mkdir -p /var/cache/japm"); // The cache directory for japm might not exist so we need to first create it using the mkdir command

    char *command = malloc(sizeof(char) * (strlen(url) + strlen("curl -s -o /var/cache/japm/") + strlen(package_name) + 1));
    strcpy(command, "curl -s -o /var/cache/japm/");
    strcat(command, package_name);
    strcat(command, " ");
    strcat(command, url);
    system(command);
    free(command);
}
