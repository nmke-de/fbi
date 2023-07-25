#include <unistd.h>

#include "print/print.h"

#include "fbi.h"

#define sysp(args) execvp(*(args), (args))

int hg_clone(char *packagename) {
	pid_t child = fork();
	if (child < 0) {
		logln("Could not spawn child process - exiting.");
		_exit(1);
	} else if (child == 0) {
		sysp((char * const*) cargs("hg", "clone", packagename));
		logln("Could not hg clone ", packagename, " - exiting.");
		_exit(2);
	}

	// Wait
	int wstatus;
	waitpid(pid, &wstatus, 0);
	if (WEXITSTATUS(wstatus) != 0)
		return 0;
	return 1;
}
