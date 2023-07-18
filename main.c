#include <stdlib.h>

#include "helpers.h"

extern void test_loads(int num_passes);
extern void test_stores(int num_passes);
extern void test_condmoves(int num_passes);
extern void test_slts(int num_passes);
extern void test_shifts(int num_passes);
extern void test_bitwise(int num_passes);
extern void test_multiplications(int num_passes);
extern void test_arithmetics(int num_passes);

int main(void)
{
	const int num_passes = 1000 * 1000;
	mxu_enable(true, false);

	srand(0);

	test_loads(num_passes);
	test_stores(num_passes);
	test_condmoves(num_passes);
	test_slts(num_passes);
	test_shifts(num_passes);
	test_bitwise(num_passes);
	test_multiplications(num_passes);
	test_arithmetics(num_passes);
	return 0;
}
