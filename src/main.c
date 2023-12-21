#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "fbi.h"

#define opt(o) (strcmp(argv[i], o) == 0)
#define lappend(li, flag) ((li)[++next_empty] = (flag))

int main(int argc, char **argv) {
	if (argc == 1) {
		return help("");
	}

	// Change working directory
	go_to_work();

	char *url = "";
	int ok = 0;
	int (*fetch)(char *) = default_fetch;
	int (*build)(char *) = default_build;
	int (*install)(char *) = default_install;
	char *fetch_arg = NULL;
	char *build_arg = NULL;
	char *install_arg = NULL;
	const char *registry_file = "fbi_registry";

	// Argparse
	if (strcmp(argv[1], "update") == 0) {
		return update(registry_file);
	} else if (strcmp(argv[1], "help") == 0) {
		return help(argc >= 3 ? argv[2] : "");
	}
	// list flags = lnew(1);
	char *flags[argc];
	int next_empty = 0;
	for (int i = 1; i < argc; i++)
		if (*(argv[i]) != '-') {
			url = argv[i];
			flags[0] = url;
		} else if (opt("-git")) {
			fetch = git_clone;
			lappend(flags, argv[i]);
		} else if (opt("-hg")) {
			fetch = hg_clone;
			lappend(flags, argv[i]);
		} else if (opt("-go")) {
			build = go_build;
			lappend(flags, argv[i]);
		} else if (opt("-make")) {
			build = make;
			lappend(flags, argv[i]);
		} else if (opt("-tar")) {
			build = tar;
			lappend(flags, argv[i]);
		} else if (opt("-vlang")) {
			build = vlang_build;
			lappend(flags, argv[i]);
		} else if (opt("-zip")) {
			build = zip;
			lappend(flags, argv[i]);
		} else if (opt("-cd-install")) {
			install = chdir_install;
			lappend(flags, argv[i]);
		} else if (opt("-go-install")) {
			install = go_install;
			lappend(flags, argv[i]);
		} else if (opt("-make-install")) {
			install = make_install;
			lappend(flags, argv[i]);
		} else if (i + 1 == argc) {
			logln("Syntax: ", basename(argv[0]), " [OPTIONS] <url>");
			return -1;
		} else if (opt("-f")) {
			fetch = custom;
			lappend(flags, argv[i]);
			fetch_arg = argv[++i];
			lappend(flags, argv[i]);
		} else if (opt("-b")) {
			build = custom;
			lappend(flags, argv[i]);
			build_arg = argv[++i];
			lappend(flags, argv[i]);
		} else if (opt("-i")) {
			install = custom;
			lappend(flags, argv[i]);
			install_arg = argv[++i];
			lappend(flags, argv[i]);
		} else {
			logln("Syntax: ", basename(argv[0]), " [OPTIONS] <url>");
			return -1;
		}
	if (fetch_arg == NULL)
		fetch_arg = url;
	if (build_arg == NULL)
		build_arg = basename(url);
	if (install_arg == NULL)
		install_arg = basename(url);

	// Fetch
	ok = fetch(fetch_arg);
	if (!ok) {
		logln("Error when fetching ", fetch_arg, ".");
		return 1;
	}
	go_to_work();

	// Register new entry
	reg(registry_file, flags, next_empty + 1);
	// lfree(&flags);

	// Build
	ok = build(build_arg);
	if (!ok) {
		logln("Error when building ", build_arg, ".");
		return 2;
	}
	go_to_work();

	// Install
	ok = install(install_arg);
	if (!ok) {
		logln("Error when installing ", install_arg, ".");
		return 3;
	}

	return 0;
}
