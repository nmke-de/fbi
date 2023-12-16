#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "fbi.h"
/*
Copy file at src to dst. Returns 1 if successful, 0 if not.
*/
int copy(const char *dst, const char *src) {
	int rfd = open(src, 0);
	int wfd = open(dst, O_CREAT | O_WRONLY, 0755);
	int result = 1;
	if (wfd < 0 || rfd < 0) {
		result = 0;
		goto close_files;
	}

	// Read file size
	long len = lseek(rfd, 0, SEEK_END);
	lseek(rfd, 0, SEEK_SET);

	// Copy data
	char *buffer = malloc(sizeof(char) * (1 << 16));
	if (buffer == NULL)
		log("Oh!\n");
	int rl = 0;
	long l = 0;
	for (; l < len; l += rl) {
		rl = read(rfd, buffer, (1 << 16));
		if (rl == 0)
			break;
		write(wfd, buffer, rl);
	}

	// Check read / written length
	result = (l == len);

	// Close
	free(buffer);
close_files:
	close(wfd);
	close(rfd);

	// Return
	return result;
}
