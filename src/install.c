#include "db.h"
#include "deps.h"
#include "out.h"
#include "pkg.h"
#include "tar.h"
#include "xfunc.h"
#include <stdio.h>
#include <stdlib.h>

int pkg_install(char *argument) {
  pkg_t *pkg = new_pkg();
  if (pkg == NULL) {
    perror("Failed to create package");
    return EXIT_FAILURE;
  }

  FILE *fp = xfopen(argument, "r");
  if (fp == NULL) {
    perror("Failed to open file");
    free_pkg(pkg);
    return EXIT_FAILURE;
  }

  pkg->info = tar_read_file_buf(fp, "pico-package");
  if (pkg->info == NULL) {
    perror("Failed to read package info");
    free_pkg(pkg);
    return EXIT_FAILURE;
  }
  fclose(fp);

  pkg_init_buf(pkg->info, pkg);
  printf("Selecting %s:%s (%s)\n", pkg->name, pkg->arch, pkg->version);

  if (pkg->state & PICO_PKG_INIT_FAIL) {
    error_msg("%s is not a package.\n", argument);
    free_pkg(pkg);
    return EXIT_FAILURE;
  }

  if (db_exists(pkg->name)) {
    error_msg("%s is already installed", pkg->name);
    free_pkg(pkg);
    return EXIT_SUCCESS;
  }

  if(pkg_check_arch(pkg)) {
    error_msg("%s have foreign architecture: %s", pkg->name, pkg->arch);
    free_pkg(pkg);
    return EXIT_SUCCESS;
  }

  pkg_satisfy_dependencies(pkg);
  if (pkg->state & PICO_PKG_DEPS_FAIL) {
    free_pkg(pkg);
    return EXIT_SUCCESS;
  }

  pkg_solve_conflicts(pkg);
  if (pkg->state &
      PICO_PKG_CONFS_FAIL) {
    free_pkg(pkg);
    return EXIT_SUCCESS;
  }

  fp = xfopen(argument, "r");
  pkg->list = tar_extract_source(fp);
  fclose(fp);

  db_add(pkg);

  free_pkg(pkg);

  return EXIT_SUCCESS;
}