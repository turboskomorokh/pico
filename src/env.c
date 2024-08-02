#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "out.h"
#include "xfunc.h"

int is_root() {
    return getuid() == 0;
}

char *read_file(FILE *fp) {
    long f_size;
    size_t r_size;
    char *buf;

    xfseek(fp, 0, SEEK_END);

    f_size = ftell(fp);
    if (f_size < 0) {
        error_msg("read_file() error: unable to get file length");
        return NULL;
    }

    xfseek(fp, 0, SEEK_SET);

    buf = xmalloc(f_size + 1);

    r_size = fread(buf, 1, f_size, fp);
    if (r_size != f_size) {
        error_msg("read_file() error: expected %ld bytes, got %zu bytes\n", f_size, r_size);
        free(buf);
        return NULL;
    }

    buf[f_size] = '\0';

    return buf;
}

char *string_compose(const char *f, ...)
{
  va_list ar;
  va_start(ar, f);
  char *buf = NULL;
  size_t s_size = vsnprintf(NULL, 0, f, ar) + 1;
  va_end(ar);
  buf = (char *)xmalloc(s_size);

  va_start(ar, f);
  vsnprintf(buf, s_size, f, ar);
  va_end(ar);
  return buf;
}
