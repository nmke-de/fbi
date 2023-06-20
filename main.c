#include <unistd.h>
#include <string.h>
#include "print/print.h"

#include "fbi.h"

int main(int argc, char **argv) {
	char *url;
	char *packagename;
	int ok = 0;
	int (*fetch) (char *) = default_fetch;
	int (*build) (char *) = default_build;
	int (*install) (char *) = default_install;
	
	// Argparse
	url = argv[1]; //temporary
	packagename = basename(url);
	
	// Fetch
	ok = fetch(url);
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
