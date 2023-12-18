#include "fbi.h"

int tar(char *packagename) {
	return sysdo("tar", "xf", packagename);
}
