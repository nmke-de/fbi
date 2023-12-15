#include "fbi.h"

int hg_clone(char *packagename) {
	return sysdo("hg", "clone", packagename);
}
