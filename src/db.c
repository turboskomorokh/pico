#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include "common.h"
#include "config.h"

#include "env.h"
#include "out.h"
#include "pkg.h"
#include "xfunc.h"

int db_add(pkg_t *pkg) {
  int r = EXIT_SUCCESS;
  char *dbp = string_compose(CONFIG_DB_DIR "/%s", pkg->name);
  char *dbi = string_compose("%s/info", dbp, pkg->name);
  char *dbl = string_compose("%s/list", dbp, pkg->name);

  mkdir(dbp, 0755);

  FILE *dbifp = xfopen(dbi, "w");
  FILE *dblfp = xfopen(dbl, "w");

  if (fputs(pkg->info, dbifp) == EOF || fputs(pkg->list, dblfp) == EOF) {
    perror_msg("db_add() %s", pkg->name);
    pkg->state |= PICO_PKG_DB_FAIL;
    r = EXIT_FAILURE;
  }

  fclose(dbifp);
  fclose(dblfp);

  free(dbp);
  free(dbi);
  free(dbl);

  return r;
}

int db_remove(pkg_t *pkg) {
  int r = EXIT_SUCCESS;
  char *dbp = string_compose(CONFIG_DB_DIR "/%s", pkg->name);
  char *dbi = string_compose("%s/info", dbp);
  char *dbl = string_compose("%s/list", dbp);

  if (remove(dbi) != EXIT_SUCCESS 
      || remove(dbl) != EXIT_SUCCESS
      || rmdir(dbp) != EXIT_SUCCESS) {
    perror_msg("db_remove() %s", pkg->name);
    r = EXIT_FAILURE;
  }

  free(dbi);
  free(dbl);
  free(dbp);

  return r;
}

int db_exists(const char *pkg_name) {
  pkg_t *db_pkg = new_pkg();
  char *dbi = string_compose(CONFIG_DB_DIR "/%s/info", pkg_name);
  FILE *dbifp = fopen(dbi, "r");
  if (!dbifp) {
    free(dbi);
    free_pkg(db_pkg);
    return FALSE;
  }
  db_pkg->info = read_file(dbifp);

  fclose(dbifp);
  free(dbi);

  pkg_init_buf(db_pkg->info, db_pkg);
  if (db_pkg->state & PICO_PKG_INIT_FAIL)
    db_pkg->state |= PICO_PKG_DB_FAIL;

  return db_pkg->state == PICO_PKG_OK;
}

pkg_t *db_get(char *pkg_name) {
  pkg_t *db_pkg = new_pkg();
  char *dbi = string_compose(CONFIG_DB_DIR "/%s/info", pkg_name);
  char *dbl = string_compose(CONFIG_DB_DIR "/%s/list", pkg_name);

  FILE *dbifp = fopen(dbi, "r");
  if (dbifp == NULL) {
    free(dbi);
    free(dbl);
    db_pkg->state |= PICO_PKG_DB_FAIL;
    return db_pkg;
  }
  FILE *dblfp = xfopen(dbl, "r");

  db_pkg->info = read_file(dbifp);
  db_pkg->list = read_file(dblfp);

  fclose(dbifp);
  fclose(dblfp);

  pkg_init_buf(db_pkg->info, db_pkg);
  if (db_pkg->state & PICO_PKG_INIT_FAIL)
    db_pkg->state |= PICO_PKG_DB_FAIL;

  free(dbi);
  free(dbl);

  return db_pkg;
}
