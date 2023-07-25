#include <sys/wait.h>
#include <unistd.h>
#include "print/print.h"

#include "fbi.h"

int curl(const char *url) {
	pid_t pid = fork();
	if (pid < 0)
		return 0;
	else if (pid == 0) {
		sysp((char * const*) cargs("curl", "-LO", url));
		_exit(1);
	}
	int wstatus;
	waitpid(pid, &wstatus, 0);
	if (WEXITSTATUS(wstatus) != 0)
		return 0;
	return 1;
}
