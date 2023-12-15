#include "fbi.h"

int git_clone(char *packagename) {
	return sysdo("git", "clone", "--recurse-submodules", packagename);
}
