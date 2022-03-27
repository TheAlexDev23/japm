#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int http_req(char*url) 
{
    char*command = "/usr/bin/curl -o - -s -w \"%%{http_code}\" ";

    strcat(command, url);
    FILE *fp = popen(command, "r");

    if (fp == NULL) 
    {
        printf("Someting went wrong\n");
        exit(7);
    }

    char*response_code = malloc(8);
 
    fgets(response_code, sizeof(response_code), fp);

    pclose(fp);

    return atoi(response_code);
}

void 
download_package(char*url, char*package_name)
{
    char*_package_name = malloc(sizeof(package_name));
    strcpy(_package_name, package_name);

    system("/usr/bin/mkdir -p /var/cache/japm");

    char*removal_command = strcat("/usr/bin/rm /var/cache/japm/", _package_name);

    system(removal_command);

    char*downloading_command1 = strcat("/usr/bin/curl ", url);
    char*downloading_command2 = strcat(">> /var/cache/japm/", package_name);
    char*downloading_command = strcat(downloading_command1, downloading_command2);
    system(downloading_command);
}