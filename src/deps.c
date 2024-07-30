#include "db.h"
#include "pkg.h"
#include "xfunc.h"
#include "env.h"
#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char *pkg_solve_dependencies(pkg_t *pkg) {
  char *ds, *d, *d_failed;

  ds = xstrdup(pkg->depends);

  d_failed = xmalloc(strlen(pkg->depends));
  d_failed[0] = '\0';

  d = strtok(ds, " ,");
  while (d != NULL) {
    if (!db_exists(d)) {
      size_t d_size = strlen(d_failed) + strlen(d) + 2;
      d_failed = xrealloc(d_failed, d_size);
      strcat(d_failed, d);
      strcat(d_failed, " ");
    }
    d = strtok(NULL, " ,");
  }

  free(ds);

  if (d_failed[0] != '\0')
    pkg->state |= PICO_PKG_DEPS_FAIL;

  return d_failed;
}

char *pkg_solve_conflicts(pkg_t *pkg) {
  char *cs, *c, *c_failed;

  cs = xstrdup(pkg->conflicts);

  c_failed = xmalloc(strlen(pkg->conflicts));
  c_failed[0] = '\0';

  c = strtok(cs, " ,");
  while (c != NULL) {
    if (db_exists(c)) {
      size_t d_size = strlen(c_failed) + strlen(c) + 2;
      c_failed = xrealloc(c_failed, d_size);
      strcat(c_failed, c);
      strcat(c_failed, " ");
    }
    c = strtok(NULL, " ,");
  }

  free(cs);

  if (c_failed[0] != '\0')
    pkg->state |= PICO_PKG_CONFS_FAIL;

  return c_failed;
}

int pkg_solve_removal_dependencies(pkg_t *pkg) {
  size_t d_len = strlen(pkg->depends) + 1;
  char *d_failed = xmalloc(d_len);

  d_failed[0] = '\0';

  DIR *dir = xopendir(CONFIG_DB_DIR);

  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 &&
        strcmp(entry->d_name, pkg->name) != 0) {
      char *full_path = string_compose(CONFIG_DB_DIR "/%s", entry->d_name);

      struct stat st;
      if (stat(full_path, &st) != 0 || !S_ISDIR(st.st_mode)) {
        free(full_path);
        continue;
      }

      pkg_t *pkg_db = db_get(entry->d_name);
      if (pkg_db && pkg_db->depends && strstr(pkg_db->depends, pkg->name)) {
        size_t new_size = d_len + strlen(pkg_db->name) + 2;
        char *temp = xrealloc(d_failed, new_size);

        d_failed = temp;
        if (d_failed[0] != '\0')
          strcat(d_failed, " ");
        strcat(d_failed, pkg_db->name);
      }
      free(full_path);
    }
  }

  if (d_failed[0] != '\0') {
    printf("These packages depend on %s:\n%s\n", pkg->name, d_failed);
    pkg->state |= PICO_PKG_DEPS_FAIL;
    goto error;
  }

  closedir(dir);
  free(d_failed);

  return EXIT_SUCCESS;

error:
  closedir(dir);
  free(d_failed);

  return EXIT_FAILURE;
}
