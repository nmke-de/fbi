#ifndef FBI_H
#define FBI_H

// Auxiliaries
char *basename(char *);
int copy(const char *, const char *);
int curl(const char *);

// Defaults
int default_build(char *);
int default_fetch(char *);
int default_install(char *);

// Fetch methods
int git_clone(char *);
int hg_clone(char *);

// Build methods
int make(char *);

// Install methods
int make_install(char *);

#endif
