#ifndef _PICO_DEPS_H_
#define _PICO_DEPS_H_

#include "pkg.h"

char *pkg_solve_dependencies(pkg_t *pkg);
int pkg_solve_conflicts(pkg_t *pkg);
char *pkg_solve_removal_dependencies(pkg_t *pkg);

#endif