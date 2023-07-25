#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "print/print.h"

#include "fbi.h"

#define opt(o) (strcmp(argv[i], o) == 0)

int main(int argc, char **argv) {
	char *url = "";
	char *packagename;
	int ok = 0;
	int (*fetch) (char *) = default_fetch;
	int (*build) (char *) = default_build;
	int (*install) (char *) = default_install;
	char *custom_fetch = "";
	char *custom_build = "";
	char *custom_install = "";
	
	// Argparse
	for (int i = 1; i < argc; i++)
		if (*(argv[i]) != '-')
			url = argv[i];
		else if (opt("-git"))
			fetch = git_clone;
		else if (opt("-hg"))
			fetch = hg_clone;
		else if (opt("-make"))
			build = make;
		else if (opt("-make-install"))
			install = make_install;
		else if (i + 1 == argc) {
			logln("Syntax: ", basename(argv[0]), " [OPTIONS] <url>");
			return -1;
		} else if (opt("-f")) {
			fetch = system;
			custom_fetch = argv[++i];
		} else if (opt("-b")) {
			build = system;
			custom_build = argv[++i];
		} else if (opt("-i")) {
			install = system;
			custom_build = argv[++i];
		} else {
			logln("Syntax: ", basename(argv[0]), " [OPTIONS] <url>");
			return -1;
		}
	// url = argv[1] ? argv[1] : ""; //temporary
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
