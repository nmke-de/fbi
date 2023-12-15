#include <sys/stat.h>

#include "fbi.h"

int default_build(char *packagename) {
	return !chmod(packagename, 0755);
}
