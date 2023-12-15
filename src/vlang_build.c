#include <unistd.h>

#include "fbi.h"

int vlang_build(char *packagename) {
	chdir(packagename);
	return sysdo("v", "-prod", ".");
}
