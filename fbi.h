#ifndef FBI_H
#define FBI_H

#include <stddef.h>
#include "print/print.h"

// Auxiliaries
void assert(int, const char *);
char *basename(char *);
char *cat(const char *);
int copy(const char *, const char *);
int curl(const char *);
void go_to_work();
void reg(const char *, char **, int);
int sysdo_internal(char * const *args);

int update(const char *);

// Custom - applicable for fetch, build and install. Not very secure.
int custom(char *);

// Defaults
int default_build(char *);
int default_fetch(char *);
int default_install(char *);

// Update methods
int git_pull(char *);
int hg_pull(char *);

// Fetch methods
int git_clone(char *);
int hg_clone(char *);

// Build methods
int go_build(char *);
int make(char *);
int vlang_build(char *);

// Install methods
int chdir_install(char *);
int go_install(char *);
int make_install(char *);

// Macros
#define sysp(args) execvp(*(args), (args))
#define sysdo(...) sysdo_internal((char * const*) cargs(__VA_ARGS__))

#endif
