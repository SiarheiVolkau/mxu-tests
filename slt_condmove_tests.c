#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

static int32_t s32max(int32_t l, int32_t r)
{
	int32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"s32max $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static int32_t s32min(int32_t l, int32_t r)
{
	int32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"s32min $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

// actually it's signed compare but for representing
// vector unsigned type is used here
static uint32_t d16max(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"d16max $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

// actually it's signed compare but for representing
// vector unsigned type is used here
static uint32_t d16min(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"d16min $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

// actually it's signed compare but for representing
// vector unsigned type is used here
static uint32_t q8max(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"q8max $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

// actually it's signed compare but for representing
// vector unsigned type is used here
static uint32_t q8min(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"q8min $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static int32_t s32cps(int32_t l, int32_t r)
{
	int32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"s32cps $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

// actually it's signed compare but for representing
// vector unsigned type is used here
static uint32_t d16cps(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"d16cps $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static uint32_t s32movz(uint32_t init, uint32_t cond, uint32_t val)
{
	int32_t ret = init;
	asm volatile (
		"s32i2m $xr1, %[ret]\n"
		"s32i2m $xr2, %[cond]\n"
		"s32i2m $xr3, %[val]\n"
		"s32movz $xr1, $xr2, $xr3\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "+r"(ret)
		: [cond] "r"(cond), [val] "r"(val)
		: "xr1", "xr2", "xr3"
	);
	return ret;
}

static uint32_t s32movn(uint32_t init, uint32_t cond, uint32_t val)
{
	int32_t ret = init;
	asm volatile (
		"s32i2m $xr1, %[ret]\n"
		"s32i2m $xr2, %[cond]\n"
		"s32i2m $xr3, %[val]\n"
		"s32movn $xr1, $xr2, $xr3\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "+r"(ret)
		: [cond] "r"(cond), [val] "r"(val)
		: "xr1", "xr2", "xr3"
	);
	return ret;
}

static uint32_t d16movz(uint32_t init, uint32_t cond, uint32_t val)
{
	int32_t ret = init;
	asm volatile (
		"s32i2m $xr1, %[ret]\n"
		"s32i2m $xr2, %[cond]\n"
		"s32i2m $xr3, %[val]\n"
		"d16movz $xr1, $xr2, $xr3\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "+r"(ret)
		: [cond] "r"(cond), [val] "r"(val)
		: "xr1", "xr2", "xr3"
	);
	return ret;
}

static uint32_t d16movn(uint32_t init, uint32_t cond, uint32_t val)
{
	int32_t ret = init;
	asm volatile (
		"s32i2m $xr1, %[ret]\n"
		"s32i2m $xr2, %[cond]\n"
		"s32i2m $xr3, %[val]\n"
		"d16movn $xr1, $xr2, $xr3\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "+r"(ret)
		: [cond] "r"(cond), [val] "r"(val)
		: "xr1", "xr2", "xr3"
	);
	return ret;
}

static uint32_t q8movz(uint32_t init, uint32_t cond, uint32_t val)
{
	int32_t ret = init;
	asm volatile (
		"s32i2m $xr1, %[ret]\n"
		"s32i2m $xr2, %[cond]\n"
		"s32i2m $xr3, %[val]\n"
		"q8movz $xr1, $xr2, $xr3\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "+r"(ret)
		: [cond] "r"(cond), [val] "r"(val)
		: "xr1", "xr2", "xr3"
	);
	return ret;
}

static uint32_t q8movn(uint32_t init, uint32_t cond, uint32_t val)
{
	int32_t ret = init;
	asm volatile (
		"s32i2m $xr1, %[ret]\n"
		"s32i2m $xr2, %[cond]\n"
		"s32i2m $xr3, %[val]\n"
		"q8movn $xr1, $xr2, $xr3\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "+r"(ret)
		: [cond] "r"(cond), [val] "r"(val)
		: "xr1", "xr2", "xr3"
	);
	return ret;
}

void test_condmoves(int num_passes)
{
	// test S32MAX
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int32_t l = random32(), r = random32();
			uint32_t expected = l < r ? r : l;
			uint32_t got = s32max(l, r);
			if (expected != got) {
				printf("S32MAX expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MAX, pass);
	}

	// test S32MIN
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int32_t l = random32(), r = random32();
			uint32_t expected = l > r ? r : l;
			uint32_t got = s32min(l, r);
			if (expected != got) {
				printf("S32MIN expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MIN, pass);
	}

	// test D16MAX
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32(), r = random32();
			uint32_t expected;
			expected = ((int16_t)l < (int16_t)r ? r : l) & 0xffff;
			expected |= ((int32_t)(l & 0xffff0000) < (int32_t)(r & 0xffff0000) ? r : l) & 0xffff0000;
			uint32_t got = d16max(l, r);
			if (expected != got) {
				printf("D16MAX expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MAX, pass);
	}

	// test D16MIN
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32(), r = random32();
			uint32_t expected;
			expected = ((int16_t)l > (int16_t)r ? r : l) & 0xffff;
			expected |= ((int32_t)(l & 0xffff0000) > (int32_t)(r & 0xffff0000) ? r : l) & 0xffff0000;
			uint32_t got = d16min(l, r);
			if (expected != got) {
				printf("D16MIN expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MIN, pass);
	}

	// test Q8MAX
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32(), r = random32();
			uint32_t got = q8max(l, r);
			uint32_t expected = 0;
			expected |= (((int8_t)l < (int8_t)r ? r : l) & 0xff) << 0;
			l >>= 8; r >>= 8;
			expected |= (((int8_t)l < (int8_t)r ? r : l) & 0xff) << 8;
			l >>= 8; r >>= 8;
			expected |= (((int8_t)l < (int8_t)r ? r : l) & 0xff) << 16;
			l >>= 8; r >>= 8;
			expected |= (((int8_t)l < (int8_t)r ? r : l) & 0xff) << 24;
			if (expected != got) {
				printf("Q8MAX expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MAX, pass);
	}

	// test Q8MIN
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32(), r = random32();
			uint32_t got = q8min(l, r);
			uint32_t expected = 0;
			expected |= (((int8_t)l > (int8_t)r ? r : l) & 0xff) << 0;
			l >>= 8; r >>= 8;
			expected |= (((int8_t)l > (int8_t)r ? r : l) & 0xff) << 8;
			l >>= 8; r >>= 8;
			expected |= (((int8_t)l > (int8_t)r ? r : l) & 0xff) << 16;
			l >>= 8; r >>= 8;
			expected |= (((int8_t)l > (int8_t)r ? r : l) & 0xff) << 24;
			if (expected != got) {
				printf("Q8MIN expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MIN, pass);
	}

	// test S32CPS
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int32_t l = random32(), r = random32();
			uint32_t expected = r < 0 ? 0 - l : l;
			uint32_t got = s32cps(l, r);
			if (expected != got) {
				printf("S32CPS expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32CPS, pass);
	}

	// test D16CPS
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32(), r = random32();
			uint32_t got = d16cps(l, r);
			uint32_t expected = 0;
			expected |= ((int16_t)r < 0 ? 0 - (int16_t)l : (int16_t)l) & 0xffff;
			l >>= 16; r >>= 16;
			expected |= ((int16_t)r < 0 ? 0 - (int16_t)l : (int16_t)l) << 16;
			if (expected != got) {
				printf("D16CPS expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16CPS, pass);
	}

	// test S32MOVZ
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t cond = random32();
			cond = random() % 2; // to have equal EQ/NE dispersion
			uint32_t val = random32();
			uint32_t init = random32();
			uint32_t expected = cond == 0 ? val : init;
			uint32_t got = s32movz(init, cond, val);
			if (got != expected) {
				printf("S32MOVZ expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MOVZ, pass);
	}

	// test S32MOVN
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t cond = random32();
			cond = random() % 2; // to have equal EQ/NE dispersion
			uint32_t val = random32();
			uint32_t init = random32();
			uint32_t expected = cond != 0 ? val : init;
			uint32_t got = s32movn(init, cond, val);
			if (got != expected) {
				printf("S32MOVN expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MOVN, pass);
	}

	// test D16MOVZ
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t cond = random32() & 0x00010001; // to have equal EQ/NE dispersion
			uint32_t val = random32();
			uint32_t init = random32();
			uint32_t expected = 0;
			expected |= ((cond & 0xffff0000) == 0) ? val & 0xffff0000 : init & 0xffff0000;
			expected |= ((cond & 0xffff) == 0) ? val & 0xffff : init & 0xffff;
			uint32_t got = d16movz(init, cond, val);
			if (got != expected) {
				printf("D16MOVZ expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MOVZ, pass);
	}

	// test D16MOVN
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t cond = random32() & 0x00010001; // to have equal EQ/NE dispersion
			uint32_t val = random32();
			uint32_t init = random32();
			uint32_t expected = 0;
			expected |= ((cond & 0xffff0000) != 0) ? val & 0xffff0000 : init & 0xffff0000;
			expected |= ((cond & 0xffff) != 0) ? val & 0xffff : init & 0xffff;
			uint32_t got = d16movn(init, cond, val);
			if (got != expected) {
				printf("D16MOVN expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MOVN, pass);
	}

	// test Q8MOVZ
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t cond = random32() & 0x01010101; // to have equal EQ/NE dispersion
			uint32_t val = random32();
			uint32_t init = random32();
			uint32_t expected = 0;
			expected |= ((cond & 0xff000000) == 0) ? val & 0xff000000 : init & 0xff000000;
			expected |= ((cond & 0x00ff0000) == 0) ? val & 0x00ff0000 : init & 0x00ff0000;
			expected |= ((cond & 0x0000ff00) == 0) ? val & 0x0000ff00 : init & 0x0000ff00;
			expected |= ((cond & 0x000000ff) == 0) ? val & 0x000000ff : init & 0x000000ff;
			uint32_t got = q8movz(init, cond, val);
			if (got != expected) {
				printf("Q8MOVZ expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MOVZ, pass);
	}

	// test Q8MOVN
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t cond = random32() & 0x01010101; // to have equal EQ/NE dispersion
			uint32_t val = random32();
			uint32_t init = random32();
			uint32_t expected = 0;
			expected |= ((cond & 0xff000000) != 0) ? val & 0xff000000 : init & 0xff000000;
			expected |= ((cond & 0x00ff0000) != 0) ? val & 0x00ff0000 : init & 0x00ff0000;
			expected |= ((cond & 0x0000ff00) != 0) ? val & 0x0000ff00 : init & 0x0000ff00;
			expected |= ((cond & 0x000000ff) != 0) ? val & 0x000000ff : init & 0x000000ff;
			uint32_t got = q8movn(init, cond, val);
			if (got != expected) {
				printf("Q8MOVN expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MOVN, pass);
	}
}

static uint32_t s32slt(int32_t l, int32_t r)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"s32slt $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

// actually it's signed compare but for representing
// vector unsigned type is used here
static uint32_t d16slt(uint32_t l, uint32_t r)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"d16slt $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

// actually it's signed compare but for representing
// vector unsigned type is used here
static uint32_t q8slt(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"q8slt $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static uint32_t q8sltu(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"q8sltu $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}


void test_slts(int num_passes)
{
	// test S32SLT
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int32_t l = random32(), r = random32();
			uint32_t expected = l < r;
			uint32_t got = s32slt(l, r);
			if (expected != got) {
				printf("S32SLT expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32SLT, pass);
	}

	// test D16SLT
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32(), r = random32();
			uint32_t got = d16slt(l, r);
			uint32_t expected = 0;
			expected |= ((int16_t)l < (int16_t)r) << 0;
			l >>= 16; r >>= 16;
			expected |= ((int16_t)l < (int16_t)r) << 16;
			if (expected != got) {
				printf("D16SLT expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16SLT, pass);
	}

	// test Q8SLT
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32(), r = random32();
			uint32_t got = q8slt(l, r);
			uint32_t expected = 0;
			expected |= ((int8_t)l < (int8_t)r) << 0;
			l >>= 8; r >>= 8;
			expected |= ((int8_t)l < (int8_t)r) << 8;
			l >>= 8; r >>= 8;
			expected |= ((int8_t)l < (int8_t)r) << 16;
			l >>= 8; r >>= 8;
			expected |= ((int8_t)l < (int8_t)r) << 24;
			if (expected != got) {
				printf("Q8SLT expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8SLT, pass);
	}

	// test Q8SLTU
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32(), r = random32();
			uint32_t got = q8sltu(l, r);
			uint32_t expected = 0;
			expected |= ((uint8_t)l < (uint8_t)r) << 0;
			l >>= 8; r >>= 8;
			expected |= ((uint8_t)l < (uint8_t)r) << 8;
			l >>= 8; r >>= 8;
			expected |= ((uint8_t)l < (uint8_t)r) << 16;
			l >>= 8; r >>= 8;
			expected |= ((uint8_t)l < (uint8_t)r) << 24;
			if (expected != got) {
				printf("Q8SLTU expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8SLTU, pass);
	}
}
