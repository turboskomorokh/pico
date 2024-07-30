#ifndef _PICO_PKG_H_
#define _PICO_PKG_H_

#define PICO_PKG_OK         0b000000
#define PICO_PKG_INIT_FAIL  0b000001
#define PICO_PKG_DB_FAIL    0b000010
#define PICO_PKG_ARCH_FAIL  0b000100
#define PICO_PKG_DEPS_FAIL  0b001000
#define PICO_PKG_CONFS_FAIL 0b010000

#include <stdio.h>

typedef struct pkg {
  char *name, *version, *arch;
  int state;
  char *depends, *conflicts;
  char *info, *list;
} pkg_t;

pkg_t *new_pkg();
void free_pkg(pkg_t *pkg);
void print_pkg(const pkg_t *pkg);
int pkg_init_buf(char *buf, pkg_t *pkg);
int pkg_remove_source(FILE *fp);
int pkg_check_arch(pkg_t *pkg);

#endif
