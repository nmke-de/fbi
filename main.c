#include <unistd.h>
#include <string.h>

int main(int argc, char **argv) {
	char *packagename;
	// Argparse
	/**/
	
	// Fetch
	fetch(packagename);
	
	// Build
	build(packagename);
	
	// Install
	install(packagename);
	
	return 0;
}
