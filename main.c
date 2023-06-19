#include <unistd.h>
#include <string.h>
#include "print/print.h"

int main(int argc, char **argv) {
	char *packagename;
	int ok = 0;
	int (*fetch) (char *);
	int (*build) (char *);
	int (*install) (char *);
	// Argparse
	/**/
	
	// Fetch
	ok = fetch(packagename);
	if (!ok) {
		logln("Error when fetching ", packagename, ".");
		return 1;
	}
	
	// Build
	ok = build(packagename);
	if (!ok) {
		logln("Error when building ", packagename, ".");
		return 2;
	}
	
	// Install
	ok = install(packagename);
	if (!ok) {
		logln("Error when installing ", packagename, ".");
		return 3;
	}
	
	return 0;
}
