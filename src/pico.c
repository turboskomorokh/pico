#include <stdio.h>
#include <stdlib.h>

#include "pico.h"
#include "env.h"

int main(int argc, char **argv) {
  if (!argv[1] || argv[1][0] != '-')
    return EXIT_FAILURE;
  switch (argv[1][1]) {
  case 'i':
    if (!is_root()) {
      printf("Operation requires root privileges\n");
      break;
    }
    argv++;
    while (*++argv) {
      int r;
      r = pkg_install(*argv);
      if (r == EXIT_FAILURE)
        break;
    }
    break;
  case 'r':
    if (!is_root()) {
      printf("Operation requires root privileges\n");
      break;
    }
    argv++;
    while (*++argv) {
      int r;
      r = pkg_remove(*argv);
      if (r == EXIT_FAILURE)
        break;
    }
    break;
  }
}
