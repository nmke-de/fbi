#include "fbi.h"

int zip(char *packagename) {
	return sysdo("unzip", "-u", packagename);
}
