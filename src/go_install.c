#include <unistd.h>

#include "fbi.h"

int go_install(char *packagename) {
	chdir(packagename);
	return sysdo("go", "install", ".");
}
