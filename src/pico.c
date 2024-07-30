#include <stdlib.h>
#include "pico.h"

int main(int argc, char **argv) {
  if (!argv[1] || argv[1][0] != '-')
    return EXIT_FAILURE;
  switch (argv[1][1]) {
    case 'i':
      argv++;
      while(*++argv) {
        int r;
        r = pkg_install(*argv);
        if(r == EXIT_FAILURE)
          break;
      }
    break;
    case 'r':
      argv++;
      while(*++argv) {
        int r;
        r = pkg_remove(*argv);
        if(r == EXIT_FAILURE)
          break;
      }
    break;
  }
}
