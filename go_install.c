#include <unistd.h>
#include <sys/wait.h>

#include "print/print.h"

#include "fbi.h"

int go_install(char *packagename) {
	chdir(packagename);

	pid_t pid = fork();
	if (pid < 0) {
		logln("Could not spawn child process - exiting.");
		_exit(1);
	} else if (pid == 0) {
		sysp((char * const*) cargs("go", "install", "."));
		logln("Could not `go install` in ", packagename, " - exiting.");
		_exit(2);
	}

	// Wait
	int wstatus;
	waitpid(pid, &wstatus, 0);
	if (WEXITSTATUS(wstatus) != 0)
		return 0;
	return 1;
}
