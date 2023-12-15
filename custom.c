#include "fbi.h"

int custom (char *cmd) {
	return sysdo("sh", "-c", cmd);
}
