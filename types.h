#include <stddef.h>
#include <stdbool.h>

struct parse_array_str {
	size_t size;
	char **content;
};

struct parse_array_str parse_array_str_init();
bool parse_array_str_push(struct parse_array_str *array, char *elem);
