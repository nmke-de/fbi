#include <unistd.h>

#include "fbi.h"

void assert(int boolean, const char *msg) {
	if (!boolean) {
		logln(msg);
		_exit(1);
	}
}
