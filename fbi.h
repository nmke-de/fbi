#ifndef FBI_H
#define FBI_H

char *basename(char *);
int curl(const char *);

int default_build(char *);
int default_fetch(char *);
int default_install(char *);

#endif
