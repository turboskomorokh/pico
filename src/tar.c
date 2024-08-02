#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "out.h"
#include "xfunc.h"

#define TAR_SUCCESS 1
#define TAR_FAILURE 0

int tar_init(struct archive **a, FILE *ap) {
  *a = archive_read_new();

  if (*a == NULL) {
    error_die("Failed to create archive object\n");
    return TAR_FAILURE;
  }

  archive_read_support_format_tar(*a);
  archive_read_support_filter_gzip(*a);

  int r = archive_read_open_FILE(*a, ap);
  if (r != ARCHIVE_OK) {
    error_msg("Unable to open file: %s", archive_error_string(*a));
    archive_read_free(*a);
    return TAR_FAILURE;
  }
  return TAR_SUCCESS;
}
int tar_check_pkg(FILE *ap) {
  struct archive *a;
  struct archive_entry *ae;
  int r, good;

  r = tar_init(&a, ap);
  if (r == TAR_FAILURE)
    return TAR_FAILURE;

  while (archive_read_next_header(a, &ae) == ARCHIVE_OK) {
    const char *aep = archive_entry_pathname(ae);
    if (!strcmp(aep, "pico-package")) {
      good = TAR_SUCCESS;
      break;
    }
    archive_read_data_skip(a);
  }

  archive_read_free(a);

  return good == TAR_SUCCESS;
}

char *tar_read_file_buf(FILE *afp, const char *fp) {
  struct archive *a;
  struct archive_entry *ae;
  int r;
  ssize_t r_size, f_size;
  char *buf = NULL;

  if (tar_init(&a, afp) == TAR_FAILURE)
    return TAR_FAILURE;

  while (archive_read_next_header(a, &ae) == ARCHIVE_OK) {
    const char *aep = archive_entry_pathname(ae);

    if (strncmp(aep, fp, strlen(fp)) == 0) {
      f_size = archive_entry_size(ae);
      buf = xmalloc(f_size + 1);
      r_size = archive_read_data(a, buf, f_size);
      break;
    }
    archive_read_data_skip(a);
  }

  if (r_size != f_size) {
    free(buf);
    error_die("Unable to read %s: read %llu got %llu\n", fp, f_size, r_size);
    return NULL;
  }

  buf[f_size] = '\0';

  archive_read_free(a);
  xfseek(afp, 0, SEEK_SET);
  return buf;
}

char *tar_extract_source(FILE *afp) {
  struct archive *a;
  struct archive_entry *ae;
  int r;
  char *buf;

  buf = xmalloc(1);
  buf[0] = '\0';

  r = tar_init(&a, afp);

  if (r == TAR_FAILURE)
    return buf;

  while (archive_read_next_header(a, &ae) == ARCHIVE_OK) {
    const char *aep = archive_entry_pathname(ae);
    if (!strcmp(aep, "pico-package")) {
      archive_read_data_skip(a);
      continue;
    }
    r = archive_read_extract(a, ae, 0);
    if (r != ARCHIVE_OK) {
      error_msg("Unable to extract %s: %s\n", aep, archive_error_string(a));
      continue;
    }
    buf = xrealloc(buf, strlen(buf) + strlen(aep) + 2);
    strcat(buf, aep);
    strcat(buf, "\n");
  }
  buf[strlen(buf) + 1] = '\0';
  xfseek(afp, 0, SEEK_SET);
  return buf;
}