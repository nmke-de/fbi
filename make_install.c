#include <unistd.h>

#include "fbi.h"

int make_install(char *packagename) {
	chdir(packagename);
	return sysdo("make", "install");
}
