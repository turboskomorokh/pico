#ifndef _PICO_OUT_H_
#define _PICO_OUT_H_

void  error_msg(const char *f, ...);
void  error_die(const char *f, ...);
void perror_msg(const char *f, ...);
void perror_die(const char *f, ...);

#endif