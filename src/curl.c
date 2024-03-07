#include "fbi.h"

int curl(const char *url) {
	logln("curl -sLO ", url);
	return sysdo("curl", "-sLO", url);
}
