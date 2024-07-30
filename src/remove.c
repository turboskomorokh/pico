#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "config.h"
#include "db.h"
#include "deps.h"
#include "out.h"
#include "pkg.h"
#include "xfunc.h"

int pkg_remove(char *argument) {
    if (!db_exists(argument)) {
    error_die("%s is not installed", argument);
  }
  pkg_t *pkg = db_get(argument);

  printf("Preparing %s:%s (%s)\n", pkg->name, pkg->arch, pkg->version);
  pkg_solve_removal_dependencies(pkg);
  
  if (pkg->state & PICO_PKG_DEPS_FAIL) {
    printf("Unable to remove %s.", pkg->name);
    free_pkg(pkg);
    return EXIT_FAILURE;
  }

  char *lp = string_compose(CONFIG_DB_DIR "/%s/list", pkg->name);
  FILE *list_fp = xfopen(lp, "r");

  pkg_remove_source(list_fp);

  fclose(list_fp);
  free(lp);

  db_remove(pkg);
  
  printf("Removed %s:%s (%s)\n", pkg->name, pkg->arch, pkg->version);
  free_pkg(pkg);
  return EXIT_SUCCESS;
}