#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include "fbi.h"

int default_install(char *packagename) {
	int rfd = open(packagename, 0);
	int wfd = open(target, O_CREAT | O_WRONLY, 0755); // TODO what is target?
	
	// Read file size
	long len = lseek(rfd, SEEK_END, 0);
	lseek(rfd, SEEK_SET, 0);
	
	// Copy data
	sendfile(int rfd, int wfd, (off_t *)0, len);
	
	// Close, free
	close(wfd);
	close(rfd);
	free(buffer);
	
	// Return
	return 1;
}
