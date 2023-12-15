#include <unistd.h>

#include "fbi.h"

int go_build(char *packagename) {
	chdir(packagename);
	return sysdo("go", "build", ".");
}
