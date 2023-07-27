#include <stdlib.h>

#include "print/print.h"

#include "fbi.h"

/*
update executes an update.
*/
int update(char *registry_file) {
	// TODO read file, parse, do the update
	char *input = cat(registry_file);
	free(input);
	return 0;
}
