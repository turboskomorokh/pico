/*
name: toast
version: 17
arch: x86_64
depends: glibc
conflicts: musl
*/
#include "pkg.h"
#include "out.h"
#include "parse.h"
#include "xfunc.h"

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>

pkg_t *new_pkg() {
  pkg_t *pkg = xmalloc(sizeof(pkg_t));
  pkg->name = NULL;
  pkg->version = NULL;
  pkg->arch = NULL;

  pkg->state = PICO_PKG_OK;

  pkg->depends = NULL;
  pkg->conflicts = NULL;

  pkg->info = NULL;
  pkg->list = NULL;

  return pkg;
}

void free_pkg(pkg_t *pkg) {
  if (pkg == NULL)
    return;

  free(pkg->name);
  free(pkg->version);
  free(pkg->arch);
  free(pkg->depends);
  free(pkg->conflicts);
  free(pkg->info);
  free(pkg->list);

  free(pkg);
}

void print_pkg(const pkg_t *pkg) {
  printf("Name: '%s'\n", pkg->name);
  printf("Version: '%s'\n", pkg->version);
  printf("Architecture: '%s'\n", pkg->arch);
  printf("State: '%d'\n", pkg->state);
  printf("Depends: '%s'\n", pkg->depends);
  printf("Conflicts: '%s'\n", pkg->conflicts);
  printf("List: '%s'\n", pkg->list);
}

int pkg_init_buf(char *buf, pkg_t *pkg) {
  char *dup, *l;

  dup = xstrdup(buf);

  l = strtok(dup, "\n");
  while (l != NULL) {
    switch (l[0]) {
    case 'a':
      if (line_match(l, "arch"))
        pkg->arch = line_get_value(l, "arch");
      break;
    case 'd':
      if (line_match(l, "depends"))
        pkg->depends = line_get_value(l, "depends");
      break;
    case 'c':
      if (line_match(l, "conflicts"))
        pkg->conflicts = line_get_value(l, "conflicts");
      break;
    case 'n':
      if (line_match(l, "name"))
        pkg->name = line_get_value(l, "name");
      break;
    case 'v':
      if (line_match(l, "version"))
        pkg->version = line_get_value(l, "version");
      break;
    }
    l = strtok(NULL, "\n");
  }
  if (!pkg->name || !pkg->arch || !pkg->version) {
    pkg->state |= PICO_PKG_INIT_FAIL;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int pkg_check_arch(pkg_t *pkg) {
  struct utsname s_info;

  if (uname(&s_info) != EXIT_SUCCESS) {
    perror("pkg_check_arch(): uname");
    return EXIT_FAILURE;
  }
  if (strcmp(s_info.machine, pkg->arch)) {
    pkg->state |= PICO_PKG_ARCH_FAIL;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int pkg_remove_source(FILE *fp) {
  struct stat st;
  char *l = xmalloc(sizeof(char) * PATH_MAX);

  while (fgets(l, PATH_MAX, fp)) {
    l[strcspn(l, "\n")] = '\0';

    if (stat(l, &st)) {
      if (errno == ENOENT)
        continue;
      perror_msg("Can't stat %s", l);
      continue;
    }

    if (S_ISREG(st.st_mode)) {
      if (remove(l) != EXIT_SUCCESS)
        perror_msg("Can't remove %s", l);
    }
  }
  xfseek(fp, 0, SEEK_SET);

  while (fgets(l, PATH_MAX, fp)) {
    l[strcspn(l, "\n")] = '\0';

    if (stat(l, &st)) {
      if (errno == ENOENT)
        continue;
      perror_msg("Can't stat %s", l);
      continue;
    }

    if (S_ISDIR(st.st_mode)) {
      if (rmdir(l) != EXIT_SUCCESS && errno != ENOTEMPTY)
        perror_msg("Can't remove %s", l);
    }
  }
  xfseek(fp, 0, SEEK_SET);
  while (fgets(l, PATH_MAX, fp)) {
    l[strcspn(l, "\n")] = '\0';

    if (stat(l, &st)) {
      if (errno == ENOENT)
        continue;
      perror_msg("Can't stat %s", l);
      continue;
    }

    if (S_ISLNK(st.st_mode)) {
      if (remove(l) != EXIT_SUCCESS)
        perror_msg("Can't remove %s", l);
    }
  }
  xfseek(fp, 0, SEEK_SET);

  free(l);
  return EXIT_SUCCESS;
}