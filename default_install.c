#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <alloca.h>

#include "fbi.h"

int default_install(char *packagename) {
	// Determine target
	const char *target_dir = "/usr/local/bin/";
	const int target_dir_len = strlen(target_dir);
	const int packagename_len = strlen(packagename);
	const int target_len = target_dir_len + packagename_len;
	char *target = alloca(sizeof(char) * (target_len));
	strncpy(target, target_dir, target_dir_len);
	strncpy(target + target_dir_len, packagename, packagename_len);
	target[target_len] = 0;

	int wfd = open(target, O_CREAT | O_WRONLY, 0755);
	if (wfd < 0)
		return 0;
	int rfd = open(packagename, 0);
	
	// Read file size
	long len = lseek(rfd, SEEK_END, 0);
	lseek(rfd, SEEK_SET, 0);
	
	// Copy data
	char *buffer = malloc(sizeof(char) * (1 << 16));
	int rl = 0;
	long l = 0;
	for (; l < len; l += rl) {
		rl = read(rfd, buffer, (1 << 16));
		if (rl == 0)
			break;
		write(wfd, buffer, rl);
	}
	
	// Close
	close(wfd);
	close(rfd);
	free(buffer);
	
	// Return
	return !(l < len);
}
