#include <unistd.h>

#include "fbi.h"

int haredo_install(char *packagename) {
	chdir(packagename);
	return sysdo("haredo", "install");
}
