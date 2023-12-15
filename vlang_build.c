#include <unistd.h>
#include <sys/wait.h>

#include "print/print.h"

#include "fbi.h"

int vlang_build(char *packagename) {
	chdir(packagename);

	pid_t pid = fork();
	if (pid < 0) {
		logln("Could not spawn child process - exiting.");
		_exit(1);
	} else if (pid == 0) {
		sysp((char * const*) cargs("v", "-prod", "."));
		logln("Could not `v build` in ", packagename, " - exiting.");
		_exit(2);
	}

	// Wait
	int wstatus;
	waitpid(pid, &wstatus, 0);
	if (WEXITSTATUS(wstatus) != 0)
		return 0;
	return 1;
}
