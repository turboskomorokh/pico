#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "out.h"

void *xmalloc(size_t s) {
  void *p = malloc(s);
  if (!p && s)
    perror_die("xmalloc");
  return p;
}

void *xrealloc(void *p, size_t s)
{
  p = realloc(p, s);
  if (!p && s)
    perror_die("xrealloc");
  return p;
}

void *xcalloc(size_t n, size_t s)
{
  void *p = calloc(n, s);
  if (!p && n && s)
    perror_die("xcalloc");
  return p;
}

char *xstrdup(const char *s)
{
  char *sd;
  if (!s)
    error_die("xstrdup: NULL string provided\n");
  sd = strdup(s);
  if (!sd)
    perror_die("xstrdup: %s", s);
  return sd;
}

char *xstrndup(const char *s, int n)
{
  char *sd;
  if (!s)
    error_die("xstrndup: NULL string provided\n");
  sd = (char *)xmalloc(strlen(s) + 1);

  strncpy(sd, s, n);
  sd[n] = '\0';
  return sd;
}

FILE *xfopen(const char *p, const char *m) {
  FILE *fp;
  if (!(fp = fopen(p, m)))
    perror_die("xfopen: %s", p);
  return fp;
}

DIR *xopendir(const char *path) {
  DIR *dir = opendir(path);
  if (!dir)
    perror_die("Can't open %s", path);
  return dir;
}

int xfseek(FILE *fp, long int off, int w) {
  int r = fseek(fp, off, w); 
  if (r != EXIT_SUCCESS) {
    perror_die("xfseek");
  }
  return r;
}