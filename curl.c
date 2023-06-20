#include <sys/wait.h>
#include <unistd.h>
#include "print/print.h"

#define sysp(args) execvp(*(args), (args))

int curl(const char *url) {
	pid_t pid = fork();
	if (pid < 0)
		return 0;
	else if (pid == 0) {
		sysp((char * const*) cargs("curl", "-LO", url));
		_exit(1);
	}
	if (waitpid(pid, 0, 0) == -1)
		return 0;
	return 1;
}
