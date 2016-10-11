#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>

#include "parse_sh.h"

void test_trivial_single(void)
{
	struct parse_array_str a = parse_sh("a");
	CU_ASSERT(a.size == 1);
	CU_ASSERT_STRING_EQUAL(a.content[0], "a");
}

void test_trivial_double(void)
{
	struct parse_array_str a = parse_sh("a b");
	CU_ASSERT(a.size == 2);
	CU_ASSERT_STRING_EQUAL(a.content[0], "a");
	CU_ASSERT_STRING_EQUAL(a.content[1], "b");
}

void test_quoted_single(void)
{
	struct parse_array_str a = parse_sh("\"a\"");
	CU_ASSERT(a.size == 1);
	CU_ASSERT_STRING_EQUAL(a.content[0], "a");
}

void test_quoted_double(void)
{
	struct parse_array_str a = parse_sh("\"a b\"");
	CU_ASSERT(a.size == 1);
	CU_ASSERT_STRING_EQUAL(a.content[0], "a b");
}

void test_quoted_two_single(void)
{
	struct parse_array_str a = parse_sh("\"a\" \"b\"");
	CU_ASSERT(a.size == 2);
	CU_ASSERT_STRING_EQUAL(a.content[0], "a");
	CU_ASSERT_STRING_EQUAL(a.content[1], "b");
}

int main()
{
	CU_pSuite suite = NULL;

	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	suite = CU_add_suite("Suite_1", NULL, NULL);
	if (suite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	if ((CU_add_test(suite, "test of 'a'", test_trivial_single) == NULL) ||
	    (CU_add_test(suite, "test of 'a b'", test_trivial_double) == NULL) ||
	    (CU_add_test(suite, "test of '\"a\"'", test_quoted_single) == NULL) ||
	    (CU_add_test(suite, "test of '\"a b\"'", test_quoted_double) == NULL) ||
	    (CU_add_test(suite, "test of '\"a\" \"b\"'", test_quoted_two_single) == NULL))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
