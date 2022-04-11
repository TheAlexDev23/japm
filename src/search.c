#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <stdlib.h>

void download_pkg_list()
{
	system("curl https://api.github.com/repos/thealexdev23/japm-official-packages/contents/packages > /tmp/japm_temporary_package_search_results");
}

void parse_pkg_result()
{
	json_object *json_parsed_obj = json_object_from_file("/tmp/japm_temporary_package_search_results");
	
	system("touch /tmp/japm_tmp_search_res");

	system("truncate -s0 /tmp/japm_tmp_search_res");
	
	FILE *f = fopen("/tmp/japm_tmp_search_res", "w");
	
	for (int i = 0; i < json_object_array_length(json_parsed_obj); i++)
	{
		json_object *array_item = json_object_array_get_idx(json_parsed_obj, i);
		json_object *name_field = json_object_object_get(array_item, "name");

		const char *name = json_object_get_string(name_field);

		fprintf(f, "%s\n", name);
	}

	fclose(f);
}

void read_from_file(char *keyword)
{
	FILE *f = fopen("/tmp/japm_tmp_search_res", "r");
	
	if (f == NULL)
	{
		fprintf(stderr, "Some unkown error happened, aborting...");
		exit(10);
	}

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

	while ((read = getline(&line, &len, f)) != -1)  { if (strstr(line, keyword) != NULL) printf("	%s", line); }

	fclose(f);
}

void search(char *keyword)
{
	printf("\033[0;32m==>Downloading package list...\n\n\n");
	printf("\033[0m");
	download_pkg_list();
	printf("\n\n\n");
	parse_pkg_result();
	printf("\033[0;32m==>Results: \n");
	printf("\033[0m");
	read_from_file(keyword);
	printf("\033[0m");
}

