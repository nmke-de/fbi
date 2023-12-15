#include "fbi.h"

char *basename(char *path) {
	int lastslash = 0;
	for (int i = 0; path[i] != 0; i++)
		if (path[i] == '/')
			lastslash = !path[i + 1] ? i : i + 1;
	return path + lastslash;
}
