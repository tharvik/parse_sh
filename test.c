#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "parse_sh.h"

static struct parse_array_str *a;

void test_init()
{
	a = parse_array_str_init();
}

void test_free()
{
	parse_array_str_free(a);
}

void test_trivial_single()
{
	test_init();

	CU_ASSERT(parse_sh(a, "a"));
	CU_ASSERT(a->size == 1);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a");

	test_free();
}

void test_trivial_double()
{
	test_init();

	CU_ASSERT(parse_sh(a, "a b"));
	CU_ASSERT(a->size == 2);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a");
	CU_ASSERT_STRING_EQUAL(a->content[1], "b");

	test_free();
}

void test_quoted_single()
{
	test_init();

	CU_ASSERT(parse_sh(a, "\"a\""));
	CU_ASSERT(a->size == 1);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a");

	test_free();
}

void test_quoted_double()
{
	test_init();

	CU_ASSERT(parse_sh(a, "\"a b\""));
	CU_ASSERT(a->size == 1);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a b");

	test_free();
}

void test_quoted_two_single()
{
	test_init();

	CU_ASSERT(parse_sh(a, "\"a\" \"b\""));
	CU_ASSERT(a->size == 2);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a");
	CU_ASSERT_STRING_EQUAL(a->content[1], "b");

	test_free();
}

void test_escaped_single()
{
	test_init();

	CU_ASSERT(parse_sh(a, "\\a"));
	CU_ASSERT(a->size == 1);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a");

	test_free();
}

void test_escaped_space()
{
	test_init();

	CU_ASSERT(parse_sh(a, "a\\ b"));
	CU_ASSERT(a->size == 1);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a b");

	test_free();
}

void test_escaped_quote()
{
	test_init();

	CU_ASSERT(parse_sh(a, "a\\\"b"));
	CU_ASSERT(a->size == 1);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a\"b");

	test_free();
}

void test_escaped_quote_double()
{
	test_init();

	CU_ASSERT(parse_sh(a, "a\\\" b"));
	CU_ASSERT(a->size == 2);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a\"");
	CU_ASSERT_STRING_EQUAL(a->content[1], "b");

	test_free();
}

void test_quote_escaped()
{
	test_init();

	CU_ASSERT(parse_sh(a, "\"a \\b\""));
	CU_ASSERT(a->size == 1);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a b");

	test_free();
}

void test_quote_escaped_quote()
{
	test_init();

	CU_ASSERT(parse_sh(a, "\"a \\\" b\""));
	CU_ASSERT(a->size == 1);
	CU_ASSERT_STRING_EQUAL(a->content[0], "a \" b");

	test_free();
}

void test_fail_single_quote()
{
	test_init();

	CU_ASSERT_FALSE(parse_sh(a, "\""));

	test_free();
}

void test_fail_missing_end_quote()
{
	test_init();

	CU_ASSERT_FALSE(parse_sh(a, "\"a b\" \"c d"));

	test_free();
}

int main()
{
	CU_pSuite suite = NULL;

	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	suite = CU_add_suite("parse_sh", NULL, NULL);
	if (suite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	ptrdiff_t e = (ptrdiff_t) ~0;

#define add_test(n, f) \
	e &= (ptrdiff_t) CU_add_test(suite, n, f);

	add_test("test of 'a'", test_trivial_single);
	add_test("test of 'a b'", test_trivial_double);
	add_test("test of '\"a\"'", test_quoted_single);
	add_test("test of '\"a b\"'", test_quoted_double);
	add_test("test of '\"a\" \"b\"'", test_quoted_two_single);
	add_test("test of '\\a'", test_escaped_single);
	add_test("test of 'a\\ b'", test_escaped_space);
	add_test("test of 'a\\\"b'", test_escaped_quote);
	add_test("test of 'a\\\" b'", test_escaped_quote_double);
	add_test("test of '\"a \\b\"'", test_quote_escaped);
	add_test("test of '\"a \\\" b\"'", test_quote_escaped_quote);

	add_test("test fail of '\"'", test_fail_single_quote);
	add_test("test fail of '\"a b\" \"c d'", test_fail_missing_end_quote);

	if(e == 0) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
