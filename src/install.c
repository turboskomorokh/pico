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
    fclose(fp);
    free_pkg(pkg);
    return EXIT_FAILURE;
  }

  xfseek(fp, 0, SEEK_SET);

  pkg_init_buf(pkg->info, pkg);
  printf("Selecting %s:%s (%s)\n", pkg->name, pkg->arch, pkg->version);

  if (pkg->state & PICO_PKG_INIT_FAIL) {
    error_msg("%s is not a package.\n", argument);
    fclose(fp);
    free_pkg(pkg);
    return EXIT_FAILURE;
  }

  if (db_exists(pkg->name)) {
    error_msg("%s is already installed", pkg->name);
    fclose(fp);
    free_pkg(pkg);
    return EXIT_SUCCESS;
  }

  if(pkg_check_arch(pkg)) {
    error_msg("%s have foreign architecture: %s", pkg->name, pkg->arch);
    fclose(fp);
    free_pkg(pkg);
    return EXIT_SUCCESS;
  }

  char *f_deps = pkg_solve_dependencies(pkg);
  if (pkg->state & PICO_PKG_DEPS_FAIL) {
    error_msg("%s depends on:\n%s\nInstall these first.", pkg->name, f_deps);
    fclose(fp);
    free_pkg(pkg);
    free(f_deps);
    return EXIT_SUCCESS;
  }

  char *f_confs = pkg_solve_conflicts(pkg);
  if (pkg->state &
      PICO_PKG_CONFS_FAIL) {
    error_msg("%s conflicts with:\n%s", pkg->name, f_confs);
    fclose(fp);
    free_pkg(pkg);
    free(f_deps);
    free(f_confs);
    return EXIT_SUCCESS;
  }

  free(f_deps);
  free(f_confs);

  pkg->list = tar_extract_source(fp);

  db_add(pkg);

  fclose(fp);
  free_pkg(pkg);

  return EXIT_SUCCESS;
}