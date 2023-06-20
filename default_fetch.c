#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include "fbi.h"

int default_fetch(char *packagename) {
	return curl(packagename);
}
