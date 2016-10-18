#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parse_sh.h"

bool parse_sh(struct parse_array_str *array, char *to_parse)
{
	size_t const to_parse_size = strlen(to_parse);
	char *out_str = calloc(to_parse_size + 1, sizeof(*out_str));
	char *out_cursor = out_str;

#define end_and_push() { \
	out_cursor[0] = '\0'; \
	if (strcmp(out_str, "") != 0) \
		parse_array_str_push(array, out_str); \
}

#define reset_out() { \
	out_str = calloc(to_parse_size - i, sizeof(*out_str)); \
	out_cursor = out_str; \
}

	bool quotted = false;
	bool espace = false;

	for (size_t i = 0; i < to_parse_size; ++i) {
		char c = to_parse[i];

		if (espace) {
			out_cursor[0] = c;
			++out_cursor;
			espace = false;
			continue;
		} else if (c == '\\') {
			espace = true;
			continue;
		}

		if (c == '"') {
			quotted = !quotted;
			continue;
		} else if (isspace(c) && !quotted) {
			end_and_push();
			reset_out();
		} else {
			out_cursor[0] = c;
			++out_cursor;
		}
	}
	end_and_push();

	if (quotted)
		return false;

	return true;
}
