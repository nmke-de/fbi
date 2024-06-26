#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "Itoa/itoa.h"

#include "fbi.h"

#define opt(o) (strcmp(input + last_field_start, (o)) == 0)

typedef enum {
	_default,
	arg_fetch,
	arg_build,
	arg_install
} argparse_state;

// Free/idle "worker threads"
static volatile int idle = 0;
int task_success[5];
void free_task(int sig) {
	int wstatus;
	pid_t dead = wait(&wstatus);
	++idle;

	if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) < 4)
		task_success[WEXITSTATUS(wstatus)] += 1;
	else
		task_success[4] += 1;
}

/*
update executes an update.
*/
int update(const char *registry_file) {
	// Amount of child processes
	long nproc = getenv("FBI_NPROC") ? atol(getenv("FBI_NPROC")) : sysconf(_SC_NPROCESSORS_ONLN);
	if (nproc < 1)
		nproc = 1;
	idle = nproc;
	char *input = cat(registry_file);
	char *url = "";
	int (*fetch)(char *) = default_fetch;
	int (*build)(char *) = default_build;
	int (*install)(char *) = default_install;
	int ok = 0;
	char *fetch_arg = NULL;
	char *build_arg = NULL;
	char *install_arg = NULL;
	argparse_state s = _default;
	int nullfd = open("/dev/null", O_WRONLY);

	// Count update tasks
	long leninput = strlen(input);
	int task_count_max = 0;
	for (int i = 0; i < leninput; i++)
		if (input[i] == '\n')
			++task_count_max;
	// Convert task_count_max to string
	char tcm[21];
	strncpy(tcm, itoa(task_count_max, 10), 21);

	// Loop
	signal(SIGCHLD, free_task);
	int last_field_start = 0;
	int task_count = 0;
	for (int i = 0; i < leninput; i++) {
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
					else if (opt("-haredo"))
						build = haredo;
					else if (opt("-make"))
						build = make;
					else if (opt("-tar"))
						build = tar;
					else if (opt("-vlang"))
						build = vlang_build;
					else if (opt("-zip"))
						build = zip;
					else if (opt("-cd-install"))
						install = chdir_install;
					else if (opt("-go-install"))
						install = go_install;
					else if (opt("-haredo-install"))
						install = haredo_install;
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

			while (idle == 0) {
			}
			--idle;

			logln("  (", itoa(++task_count, 10), "/", tcm, ") Updating ", url);
			pid_t child = fork();
			if (child < 0) {
				logln("Error when forking.");
				return 1;
			} else if (child == 0) {
				// Less noisy task
				int logfd = dup(2);
				close(1);
				close(2);
				dup2(nullfd, 1);
				dup2(nullfd, 2);
				close(nullfd);

				// Fetch
				ok = fetch(fetch_arg);
				if (!ok) {
					free(input);
					fdprintv(logfd, cargs("L (", itoa(task_count, 10), "/", tcm, ") Fetch error with ", url, "\n"));
					close(logfd);
					_exit(1);
				}
				go_to_work();

				// Build
				ok = build(build_arg);
				if (!ok) {
					free(input);
					fdprintv(logfd, cargs("L (", itoa(task_count, 10), "/", tcm, ") Build error with ", url, "\n"));
					close(logfd);
					_exit(2);
				}
				go_to_work();

				// Install
				ok = install(install_arg);
				if (!ok) {
					free(input);
					fdprintv(logfd, cargs("L (", itoa(task_count, 10), "/", tcm, ") Install error with ", url, "\n"));
					close(logfd);
					_exit(3);
				}

				// free(input);
				fdprintv(logfd, cargs("  (", itoa(task_count, 10), "/", tcm, ") Successfully updated ", url, "\n"));
				close(logfd);
				_exit(0);
			}
		update_next:
			fetch = default_fetch;
			build = default_build;
			install = default_install;
			fetch_arg = NULL;
			build_arg = NULL;
			install_arg = NULL;
		}
	}
	close(nullfd);

	// Wait for all child processes to end
	while (idle != nproc) {
	}

	char suc[21], fet[21], bld[21], inst[21], idk[21];
	logln(strncpy(suc, itoa(task_success[0], 10), 21), " successful updates, ", strncpy(fet, itoa(task_success[1], 10), 21), " fetch errors, ", strncpy(bld, itoa(task_success[2], 10), 21), " build errors, ", strncpy(inst, itoa(task_success[3], 10), 21), " install errors and ", strncpy(idk, itoa(task_success[4], 10), 21), " unknown errors.");

	free(input);
	return !(task_success[1] + task_success[2] + task_success[3] + task_success[4]);
}
