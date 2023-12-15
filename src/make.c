#include <unistd.h>

#include "fbi.h"

int make(char *packagename) {
	chdir(packagename);
	return sysdo("make");
}
