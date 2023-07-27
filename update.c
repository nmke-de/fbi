#include <fcntl.h>
#include <unistd.h>

#include "print/print.h"

#include "fbi.h"

/*
update executes an update.
*/
int update(char *registry_file) {
	int fd = open(registry_file, 0);
	// TODO read file, parse, do the update
	close(fd);
	return 0;
}
