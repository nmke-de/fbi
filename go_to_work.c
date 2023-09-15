#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fbi.h"

void go_to_work() {
	char *workplace = getenv("FBI_WORKPLACE") ? getenv("FBI_WORKPLACE") : "/opt/fbi";
	mkdir(workplace, 0755);
	chdir(workplace);
}
