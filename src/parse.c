#include <string.h>

#include "xfunc.h"
#include "parse.h"

int line_match(const char *l, const char* t) {
  size_t t_size = strlen(t);
  return strncmp(l, t, t_size) == 0;
}

char *line_get_value(const char *l, const char *t) {
  size_t cut_size = strlen(t)+strlen(LINE_TYPE_DELIM);
  return xstrdup(l+cut_size);
}