#ifndef _PICO_PARSE_H_
#define _PICO_PARSE_H_

#define LINE_TYPE_DELIM ": "

int line_match(const char *l, const char* t);

char *line_get_value(const char *l, const char *t);

#endif