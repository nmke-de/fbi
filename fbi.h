#ifndef FBI_H
#define FBI_H

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
int copy(const char *, const char *);
int curl(const char *);
void go_to_work();

// Custom - applicable for fetch, build and install. Not very secure.
int custom(char *);

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

// Macros
#define sysp(args) execvp(*(args), (args))

#endif
