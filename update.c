#include <stdlib.h>

#include "print/print.h"

#include "fbi.h"

#define opt(o) (strcmp(input + last_field_start, (o)) == 0)

/*
update executes an update.
*/
int update(const char *registry_file) {
	// TODO read file, parse, do the update
	char *input = cat(registry_file);
	char *url = "";
	int (*fetch) (char *) = default_fetch;
	int (*build) (char *) = default_build;
	int (*install) (char *) = default_install;
	int ok = 0;
	char *fetch_arg = NULL;
	char *build_arg = NULL;
	char *install_arg = NULL;

	// Loop
	int last_field_start = 0;
	for (int i = 0; input[i] != '\0'; i++)
		if (input[i] == '\n') {
			// update on the entry
			if (fetch_arg == NULL)
				fetch_arg = url;
			if (build_arg == NULL)
				build_arg = basename(url);
			if (install_arg == NULL)
				install_arg = build_arg;	

			// Fetch
			ok = fetch(fetch_arg);
			if (!ok) {
				logln("Error when fetching ", fetch_arg, ".");
				goto update_next;
			}

			// Build
			ok = build(build_arg);
			if (!ok) {
				logln("Error when building ", build_arg, ".");
				goto update_next;
			}

			// Install
			ok = install(install_arg);
			if (!ok) {
				logln("Error when installing ", install_arg, ".");
			}
update_next:
			fetch_arg = NULL;
			build_arg = NULL;
			install_arg = NULL;
			last_field_start = i + 1;
		} else if (input[i] == '\t') {
			// Evaluate current field
			input[i] = '\0';
			if (input[last_field_start] != '-') 
				url = input + last_field_start;
			else if (opt("-git"))
				fetch = git_pull;
			else if (opt("-hg"))
				fetch = hg_pull;
			else if (opt("-make"))
				build = make;
			else if (opt("-make-install"))
				install = make_install;
			// TODO the other options
			last_field_start = i + 1;
		} else continue;

	free(input);
	return 0;
}
