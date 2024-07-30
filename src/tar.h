#ifndef _PICO_TAR_H_
#define _PICO_TAR_H_

#include <stdio.h>

int tar_check_pkg(FILE *ap);
char *tar_read_file_buf(FILE *ap, const char *fp);
char *tar_extract_source(FILE *ap);

#endif