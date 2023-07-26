#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fbi.h"

void go_to_work() {
	// TODO find a better default workplace than /tmp/fbi, preferrably in the HOME directory.
	char *workplace = getenv("FBI_WORKPLACE") ? getenv("FBI_WORKPLACE") : "/tmp/fbi";
	mkdir(workplace, 0755);
	chdir(workplace);
}
