#ifndef _PICO_ENV_H_
#define _PICO_ENV_H_

#include <stdio.h>

char *read_file(FILE *fp);
char *string_compose(const char *f, ...);
int is_root();

#endif
