#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

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
	list cids = lnew(1);
	list tasknames = lnew(1);
	int nullfd = open("/dev/null", O_WRONLY); 

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

			logln("Updating ", url);
			lappend(&tasknames, url);
			lappend(&cids, (void *) fork());
			pid_t child = (pid_t) cids.content[cids.len];
			if (child < 0) {
				logln("Error when forking.");
				return 1;
			} else if (child == 0) {
				// Less noisy task
				close(1);
				close(2);
				dup2(nullfd, 1);
				dup2(nullfd, 2);
				close(nullfd);
				lfree(&cids);
				lfree(&tasknames);

				// Fetch
				ok = fetch(fetch_arg);
				if (!ok) {
					free(input);
					_exit(1);
				}

				// Build
				ok = build(build_arg);
				if (!ok) {
					free(input);
					_exit(2);
				}

				// Install
				ok = install(install_arg);
				if (!ok) {
					free(input);
					_exit(3);
				}
				free(input);
				_exit(0);
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
	close(nullfd);

	for (int i = 0; i < cids.len; i++) {
		int wstatus;
		waitpid((pid_t) cids.content[i + 1], &wstatus, 0);
		switch (WEXITSTATUS(wstatus)) {
			case 1:
				logln("Fetch error with ", tasknames.content[i + 1]);
				break;
			case 2:
				logln("Build error with ", tasknames.content[i + 1]);
				break;
			case 3:
				logln("Install error with ", tasknames.content[i + 1]);
				break;
			default:
				logln("Successfully updated ", tasknames.content[i + 1]);
		}
	}

	free(input);
	lfree(&cids);
	lfree(&tasknames);
	return 0;
}
