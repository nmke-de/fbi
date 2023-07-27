#include <alloca.h>
#include <fcntl.h>
#include <unistd.h>

#include "print/print.h"

#include "fbi.h"

void reg(const char *registry_file, list flags) {
	int fd = open(registry_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	char **entry = alloca(sizeof(char *) * (flags.len * 2 + 1));
	for (int i = 0; i <= flags.len; i++) {
		logln(flags.content[i]);
		entry[i * 2] = flags.content[i];
		entry[i * 2 + 1] = "\t";
	}
	entry[flags.len * 2 + 1] = "\n";
	entry[flags.len * 2 + 2] = (void *) 0;
	fdprintv(fd, (const char **) entry);
	close(fd);
}
