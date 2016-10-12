#include "types.h"

#include <stdlib.h>

struct parse_array_str *parse_array_str_init()
{
	struct parse_array_str *array = malloc(sizeof(*array));

	array->content = NULL;
	array->size = 0;

	return array;
}

void parse_array_str_free(struct parse_array_str *array)
{
	free(array->content);
	free(array);
}

bool parse_array_str_push(struct parse_array_str *array, char *elem)
{
	char **allocated = realloc(array->content,
				   (array->size + 1) * sizeof(array->content));
	if (!allocated)
		return false;

	array->content = allocated;
	array->content[array->size] = elem;
	array->size++;

	return true;
}
