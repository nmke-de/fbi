#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "print/print.h"

#include "fbi.h"

#define opt(o) (strcmp(argv[i], o) == 0)

int main(int argc, char **argv) {
	if (argc == 1) {
		logln("Syntax: ", basename(argv[0]), " [OPTIONS] <url>");
		return -1;
	}

	// Change working directory
	go_to_work();

	char *url = "";
	int ok = 0;
	int (*fetch) (char *) = default_fetch;
	int (*build) (char *) = default_build;
	int (*install) (char *) = default_install;
	char *fetch_arg = NULL;
	char *build_arg = NULL;
	char *install_arg = NULL;
	char *registry_file = getenv("FBI_REGISTRY") ? getenv("FBI_REGISTRY") : "fbi_registry";
	
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
			fetch = custom;
			fetch_arg = argv[++i];
		} else if (opt("-b")) {
			build = custom;
			build_arg = argv[++i];
		} else if (opt("-i")) {
			install = custom;
			install_arg = argv[++i];
		} else {
			logln("Syntax: ", basename(argv[0]), " [OPTIONS] <url>");
			return -1;
		}
	if (fetch_arg == NULL)
		fetch_arg = url;
	if (build_arg == NULL)
		build_arg = basename(url);
	if (install_arg == NULL)
		install_arg = build_arg;
	
	// Fetch
	ok = fetch(fetch_arg);
	if (!ok) {
		logln("Error when fetching ", fetch_arg, ".");
		return 1;
	}

	// Build
	ok = build(build_arg);
	if (!ok) {
		logln("Error when building ", build_arg, ".");
		return 2;
	}
	
	// Install
	ok = install(install_arg);
	if (!ok) {
		logln("Error when installing ", install_arg, ".");
		return 3;
	}
	
	return 0;
}
