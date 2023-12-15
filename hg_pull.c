#include <unistd.h>

#include "fbi.h"

int hg_pull(char *packagename) {
	chdir(basename(packagename));
	return sysdo("hg", "pull");
}
