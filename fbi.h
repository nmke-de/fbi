#ifndef FBI_H
#define FBI_H

#include <stddef.h>

// List type
typedef struct {
	void **content;
	int len;
	int cap;
} list;

list lnew(size_t);
void lappend(list *, void *);
void lfree(list *);

// Auxiliaries
char *basename(char *);
char *cat(const char *);
int copy(const char *, const char *);
int curl(const char *);
void go_to_work();
void reg(const char *, list);

int update(char *);

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
int make(char *);

// Install methods
int make_install(char *);

// Macros
#define sysp(args) execvp(*(args), (args))

#endif
