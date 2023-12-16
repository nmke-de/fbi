#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "fbi.h"

const int chunksize = (1 << 12);

/**
Open file, read contents until EOF, then close. Return read content.
File can be non-regular file.
*/
char *cat(const char *filename) {
	char *input = NULL;
	size_t cap = 0;
	int fd = open(filename, 0);
	int rl = chunksize;
	while (rl == chunksize) {
		cap += rl;
		input = realloc(input, sizeof(char) * cap);
		rl = read(fd, input + cap - rl, chunksize);
	}
	close(fd);
	// debug(input);
	return input;
}
