//#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//#include <stdio.h>
//#include <assert.h>
#include <ctype.h>

#include "parse_sh.h"

struct parse_array_str parse_sh(char *to_parse)
{
	struct parse_array_str out = parse_array_str_init();

	size_t const to_parse_size = strlen(to_parse);
	char *out_str = calloc(to_parse_size, sizeof(*out_str));
	char *out_cursor = out_str;

#define end_and_push() { \
	out_cursor[0] = '\0'; \
	parse_array_str_push(&out, out_str); \
}

#define reset_out() { \
	out_str = calloc(to_parse_size - i, sizeof(*out_str)); \
	out_cursor = out_str; \
}

	bool quotted = false;

	for(size_t i = 0; i < to_parse_size; ++i) {
		char c = to_parse[i];

		if(c == '"') {
			quotted = !quotted;
			continue;
		} else if(isspace(c) && !quotted) {
			end_and_push();
			reset_out();
		} else {
			out_cursor[0] = c;
			++out_cursor;
		}
	}
	end_and_push();

	return out;
}
