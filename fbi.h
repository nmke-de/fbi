#ifndef FBI_H
#define FBI_H

char *basename(char *);
int copy(const char *, const char *);
int curl(const char *);

int default_build(char *);
int default_fetch(char *);
int default_install(char *);

int git_clone(char *);
int hg_clone(char *);

int make(char *);

#endif
