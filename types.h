#ifndef TYPES_H
#define TYPES_H

#include <stddef.h>
#include <stdbool.h>

struct parse_array_str {
	size_t size;
	char **content;
};

struct parse_array_str *parse_array_str_init();
void parse_array_str_free(struct parse_array_str *array);

bool parse_array_str_push(struct parse_array_str *array, char *elem);

#endif
