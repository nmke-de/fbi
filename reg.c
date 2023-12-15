#include <fcntl.h>
#include <unistd.h>

#include "fbi.h"

void reg(const char *registry_file, char **flags, int nflags) {
	int fd = open(registry_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	char *entry[nflags * 2 + 1];
	for (int i = 0; i < nflags; i++) {
		entry[i * 2] = flags[i];
		entry[i * 2 + 1] = "\t";
	}
	entry[nflags * 2 - 1] = "\n";
	entry[nflags * 2] = (void *) 0;
	fdprintv(fd, (const char **) entry);
	close(fd);
}
