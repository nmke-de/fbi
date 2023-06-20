#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <alloca.h>
#include <sys/sendfile.h>

#include "fbi.h"

int default_install(char *packagename) {
	// Determine target
	const char *target_dir = "/usr/local/bin/";
	const int target_dir_len = strlen(target_dir);
	const int packagename_len = strlen(packagename);
	const int target_len = target_dir_len + packagename_len;
	char *target = alloca(sizeof(char) * target_len);
	strncpy(target, target_dir, target_dir_len);
	strncpy(target + target_dir_len, packagename, packagename_len);

	int rfd = open(packagename, 0);
	int wfd = open(target, O_CREAT | O_WRONLY, 0755);
	
	// Read file size
	long len = lseek(rfd, SEEK_END, 0);
	lseek(rfd, SEEK_SET, 0);
	
	// Copy data
	sendfile(rfd, wfd, (off_t *)0, len);
	
	// Close
	close(wfd);
	close(rfd);
	
	// Return
	return 1;
}
