#ifndef _PICO_DB_H_
#define _PICO_DB_H_

#include "pkg.h"

int    db_add(pkg_t *pkg);
int    db_remove(pkg_t *pkg);
int    db_exists(const char *name);
pkg_t *db_get(const char *name);

#endif
