#include <stdlib.h>

#include "print/print.h"

#include "fbi.h"

#define opt(o) (strcmp(input + last_field_start, (o)) == 0)

typedef enum {
	_default,
	arg_fetch,
	arg_build,
	arg_install
} argparse_state; 

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
	argparse_state s = _default;

	// Loop
	int last_field_start = 0;
	for (int i = 0; input[i] != '\0'; i++) {
		if (input[i] == '\t' || input[i] == '\n') {
			// Evaluate current field
			char sep = input[i];
			input[i] = '\0';
			switch (s) {
				case arg_fetch:
					fetch_arg = input + last_field_start;
					s = _default;
					break;
				case arg_build:
					build_arg = input + last_field_start;
					s = _default;
					break;
				case arg_install:
					install_arg = input + last_field_start;
					s = _default;
					break;
				default:
					if (input[last_field_start] != '-') 
						url = input + last_field_start;
					else if (opt("-git"))
						fetch = git_pull;
					else if (opt("-hg"))
						fetch = hg_pull;
					else if (opt("-go"))
						build = go_build;
					else if (opt("-make"))
						build = make;
					else if (opt("-vlang"))
						build = vlang_build;
					else if (opt("-cd-install"))
						install = chdir_install;
					else if (opt("-go-install"))
						install = go_install;
					else if (opt("-make-install"))
						install = make_install;
					else if (opt("-f")) {
						fetch = custom;
						s = arg_fetch;
					} else if (opt("-b")) {
						build = custom;
						s = arg_build;
					} else if (opt("-i")) {
						install = custom;
						s = arg_install;
					}
			}
			last_field_start = i + 1;

			if (sep == '\t')
				continue;
			if (s != _default) {
				logln("Error: invalid fbi options.");
				s = _default;
				goto update_next;
			}
			
			// update on the entry
			if (fetch_arg == NULL)
				fetch_arg = url;
			if (build_arg == NULL)
				build_arg = basename(url);
			if (install_arg == NULL)
				install_arg = basename(url);

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
			fetch = default_fetch;
			build = default_build;
			install = default_install;
			fetch_arg = NULL;
			build_arg = NULL;
			install_arg = NULL;
			last_field_start = i + 1;
		}
	}

	free(input);
	return 0;
}
