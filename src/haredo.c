#include <unistd.h>

#include "fbi.h"

int haredo(char *packagename) {
	chdir(packagename);
	return sysdo("haredo");
}
