#include <string.h>

#include "fbi.h"

#define opt(o) (strcmp(o, topic) == 0)

int help(char *topic) {
	static const char *valid_topics = "\n\n\
Valid help topics:\n\
environ\n\
help\n\
options\n\
update";
	if (opt("environ")) {
		println("fbi environment variables\n\
FBI_NPROC\tdetermines the maximum amount of parallel processes run by `fbi\n\
\t\tupdate`. By default, this is $(nproc).\n\
FBI_TARGET\tinfluences the location where fbi installs binaries to, using\n\
\t\tdefault installation or the -cd-install flag. By default, this is\n\
\t\t`/usr/local/bin`.\n\
FBI_WORKPLACE\tdetermines the working directory of fbi. By default, this is\n\
\t\t`/opt/fbi`.",
			valid_topics);
	} else if (opt("help")) {
		println("fbi help\n\
Show help text on various topics.",
			valid_topics);
	} else if (opt("options")) {
		println("FETCH OPTIONS\n\
-git\tUse git clone and git pull\n\
-hg\tUse hg clone and hg pull\n\
-f <cmds>\tExecute <cmds> in a shell.\n\
default: -f 'curl <url>'\n\
\n\
BUILD OPTIONS\n\
-go\tUse go build system\n\
-make\tCall `make` in package directory\n\
-tar\tExtract tar archive\n\
-vlang\tUse vlang build system\n\
-zip\tExtract zip archive\n\
-b <cmds>\tExecute <cmds> in a shell.\n\
default: -b 'chmod 0755 <packagename>'\n\
\n\
INSTALL OPTIONS\n\
-cd-install\tInstall the binary at $FBI_WORKPLACE/<packagename>/<packagename>\n\
\t\tto $FBI_TARGET/<packagename>\n\
-go-install\tUse `go install`\n\
-make-install\tCall `make install` in the package directory.\n\
-i <cmds>\tExecute <cmds> in a shell.\n\
default: -i 'mv <packagename> $FBI_TARGET/<packagename>'",
			valid_topics);
	} else if (opt("update")) {
		println("fbi update\n\
Update every installed package. This usually happens in $(nproc) parallel\n\
processes. To change this behaviour, set the environment variable FBI_NPROC to\n\
the amount of processes the update shall be run at.\n\
For example, `FBI_NPROC=1 fbi update` can be used to effectively suspend\n\
parallel execution of an update.",
			valid_topics);
	} else {
		println("Syntax:\
\tfbi <url> [FETCH-OPTION] [BUILD-OPTION] [INSTALL-OPTION]\n\
\tfbi help [TOPIC]\n\
\tfbi update",
			valid_topics);
	}
	return -1;
}
