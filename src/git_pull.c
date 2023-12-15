#include <unistd.h>

#include "fbi.h"

int git_pull(char *packagename) {
	chdir(basename(packagename));
	return sysdo("git", "pull", "--recurse-submodules");
}
