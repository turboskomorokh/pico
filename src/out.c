#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

void error_msg(const char *f, ...)
{
  va_list args;
  va_start(args, f);
  vfprintf(stderr, f, args);
  fputc('\n', stderr);
  va_end(args);
}

void error_die(const char *f, ...)
{
  va_list args;
  va_start(args, f);
  vfprintf(stderr, f, args);
  fputc('\n', stderr);
  va_end(args);
  exit(EXIT_FAILURE);
}

void perror_msg(const char *f, ...) {
  char *serr = strerror(errno);
  va_list args;
  va_start(args, f);
  vfprintf(stderr, f, args);
  va_end(args);
  fprintf(stderr, ": %s\n", serr);
}

void perror_die(const char *f, ...) {
  char *serr = strerror(errno);
  va_list args;
  va_start(args, f);
  vfprintf(stderr, f, args);
  va_end(args);
  fprintf(stderr, ": %s\n", serr);
  exit(EXIT_FAILURE);
}
