#include <string.h>

#include "fbi.h"

#define opt(o) (strcmp(o, topic) == 0)

int help(char *topic) {
	if (opt("update")) {
		println("fbi update\n\
Update every installed package. This usually happens in $(nproc) parallel\n\
processes. To change this behaviour, set the environment variable FBI_NPROC to\n\
the amount of processes the update shall be run at.\n\
For example, `FBI_NPROC=1 fbi update` can be used to effectively suspend\n\
parallel execution of an update.");
	} else {
		println("Syntax:\
\tfbi <url> [FETCH-OPTION] [BUILD-OPTION] [INSTALL-OPTION]\n\
\tfbi help [TOPIC]\n\
\tfbi update\n\
\n\
Valid help topics:\n\
update");
	}
	return -1;
}
