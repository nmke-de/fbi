#include "fbi.h"

int default_fetch(char *packagename) {
	return curl(packagename);
}
