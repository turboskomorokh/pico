#ifndef _PICO_XFUNC_
#define _PICO_XFUNC_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void *xmalloc(size_t s);
void *xrealloc(void *p, size_t s);
void *xcalloc(size_t n, size_t s);
char *xstrdup(const char *s);
char *xstrndup(const char *s, int n);
FILE *xfopen(const char *p, const char *m);
DIR *xopendir(const char *path);
int xfseek(FILE *fp, long int off, int w);

#endif