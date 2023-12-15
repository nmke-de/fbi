#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <alloca.h>

#include "fbi.h"

int default_install(char *packagename) {
	// Determine target
	const char *target_dir = getenv("FBI_TARGET") ? getenv("FBI_TARGET") : "/usr/local/bin";
	const int target_dir_len = strlen(target_dir) + 1;
	const int packagename_len = strlen(packagename);
	const int target_len = target_dir_len + packagename_len;
	char *target = alloca(sizeof(char) * (target_len));
	strncpy(target, target_dir, target_dir_len - 1);
	target[target_dir_len - 1] = '/';
	strncpy(target + target_dir_len, packagename, packagename_len);
	target[target_len] = 0;

	// Copy file
	int result = copy(target, packagename);
	
	// Remove intermediary file to free disk space, unless installed to current working directory
	char cwd[1 << 10];
	if (strcmp(getcwd(cwd, sizeof(cwd)), target_dir) != 0)
		unlink(packagename);
	
	// Return
	return result;
}
