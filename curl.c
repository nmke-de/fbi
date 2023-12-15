#include "fbi.h"

int curl(const char *url) {
	return sysdo("curl", "-LO", url);
}
