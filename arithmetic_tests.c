#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

#define GEN_D32ADD_insn(aptn2) \
static void d32add##aptn2(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"d32add $xr1, $xr2, $xr1, $xr2, %[ptn]\n" \
		"s32std $xr1, %U[a]\n" \
		"s32std $xr2, %U[d]\n" \
		: [a] "=*m"(*a), [d] "=*m"(*d) \
		: [b] "r"(b), [c] "r"(c), [ptn] "i"(aptn2) \
		: "xr1", "xr2" \
	); \
}

GEN_D32ADD_insn(0)
GEN_D32ADD_insn(1)
GEN_D32ADD_insn(2)
GEN_D32ADD_insn(3)

typedef void (*d32add_proto_t)(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c);
static void d32add(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		d32add0, d32add1, d32add2, d32add3,
	};
	fns[aptn2](a, d, b, c);
}

static void d32addc(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, uint32_t carry)
{
	s32i2m(16, carry | 1u);
	asm volatile (
		"s32ldd $xr1, %U[a]\n"
		"s32ldd $xr2, %U[d]\n"
		"s32i2m $xr3, %[b]\n"
		"s32i2m $xr4, %[c]\n"
		"d32addc $xr1, $xr2, $xr3, $xr4\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr2, %U[d]\n"
		: [a] "=*m"(*a), [d] "=*m"(*d)
		: [b] "r"(b), [c] "r"(c)
		: "xr1", "xr2", "xr3", "xr4"
	);
}

#define GEN_DQxxAxx_insn(code, aptn2) \
static void _##code##aptn2(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"s32ldd $xr3, %U[a]\n" \
		"s32ldd $xr4, %U[d]\n" \
		#code " $xr3, $xr4, $xr1, $xr2, %[ptn]\n" \
		"s32std $xr3, %U[a]\n" \
		"s32std $xr4, %U[d]\n" \
		: [a] "+*m"(*a), [d] "+*m"(*d) \
		: [b] "r"(b), [c] "r"(c), [ptn] "i"(aptn2) \
		: "xr1", "xr2", "xr3", "xr4" \
	); \
}

GEN_DQxxAxx_insn(d32acc, 0)
GEN_DQxxAxx_insn(d32acc, 1)
GEN_DQxxAxx_insn(d32acc, 2)
GEN_DQxxAxx_insn(d32acc, 3)

static void d32acc(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		_d32acc0, _d32acc1, _d32acc2, _d32acc3,
	};
	fns[aptn2](a, d, b, c);
}

GEN_DQxxAxx_insn(d32accm, 0)
GEN_DQxxAxx_insn(d32accm, 1)
GEN_DQxxAxx_insn(d32accm, 2)
GEN_DQxxAxx_insn(d32accm, 3)

static void d32accm(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		_d32accm0, _d32accm1, _d32accm2, _d32accm3,
	};
	fns[aptn2](a, d, b, c);
}

GEN_DQxxAxx_insn(d32asum, 0)
GEN_DQxxAxx_insn(d32asum, 1)
GEN_DQxxAxx_insn(d32asum, 2)
GEN_DQxxAxx_insn(d32asum, 3)

static void d32asum(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		_d32asum0, _d32asum1, _d32asum2, _d32asum3,
	};
	fns[aptn2](a, d, b, c);
}

#define GEN_Q16ADD_insn(aptn2,optn2) \
static void q16add##aptn2##optn2(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"q16add $xr1, $xr2, $xr1, $xr2, %[aptn], %[optn]\n" \
		"s32std $xr1, %U[a]\n" \
		"s32std $xr2, %U[d]\n" \
		: [a] "=*m"(*a), [d] "=*m"(*d) \
		: [b] "r"(b), [c] "r"(c), [aptn] "i"(aptn2), [optn] "i"(optn2) \
		: "xr1", "xr2" \
	); \
}

GEN_Q16ADD_insn(0,0)
GEN_Q16ADD_insn(0,1)
GEN_Q16ADD_insn(0,2)
GEN_Q16ADD_insn(0,3)
GEN_Q16ADD_insn(1,0)
GEN_Q16ADD_insn(1,1)
GEN_Q16ADD_insn(1,2)
GEN_Q16ADD_insn(1,3)
GEN_Q16ADD_insn(2,0)
GEN_Q16ADD_insn(2,1)
GEN_Q16ADD_insn(2,2)
GEN_Q16ADD_insn(2,3)
GEN_Q16ADD_insn(3,0)
GEN_Q16ADD_insn(3,1)
GEN_Q16ADD_insn(3,2)
GEN_Q16ADD_insn(3,3)

static void q16add(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2, int optn2)
{
	const d32add_proto_t fns[4][4] = {
		{ q16add00, q16add01, q16add02, q16add03, },
		{ q16add10, q16add11, q16add12, q16add13, },
		{ q16add20, q16add21, q16add22, q16add23, },
		{ q16add30, q16add31, q16add32, q16add33, },
	};
	fns[aptn2][optn2](a, d, b, c);
}

GEN_DQxxAxx_insn(q16acc, 0)
GEN_DQxxAxx_insn(q16acc, 1)
GEN_DQxxAxx_insn(q16acc, 2)
GEN_DQxxAxx_insn(q16acc, 3)

static void q16acc(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		_q16acc0, _q16acc1, _q16acc2, _q16acc3,
	};
	fns[aptn2](a, d, b, c);
}

GEN_DQxxAxx_insn(q16accm, 0)
GEN_DQxxAxx_insn(q16accm, 1)
GEN_DQxxAxx_insn(q16accm, 2)
GEN_DQxxAxx_insn(q16accm, 3)

static void q16accm(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		_q16accm0, _q16accm1, _q16accm2, _q16accm3,
	};
	fns[aptn2](a, d, b, c);
}

GEN_DQxxAxx_insn(d16asum, 0)
GEN_DQxxAxx_insn(d16asum, 1)
GEN_DQxxAxx_insn(d16asum, 2)
GEN_DQxxAxx_insn(d16asum, 3)

static void d16asum(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		_d16asum0, _d16asum1, _d16asum2, _d16asum3,
	};
	fns[aptn2](a, d, b, c);
}

static uint32_t d16avg(uint32_t l, uint32_t r)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"d16avg $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static uint32_t d16avgr(uint32_t l, uint32_t r)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"d16avgr $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static void q16scop(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c)
{
	asm volatile (
		"s32i2m $xr1, %[b]\n"
		"s32i2m $xr2, %[c]\n"
		"q16scop $xr1, $xr2, $xr1, $xr2\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr2, %U[d]\n"
		: [a] "=m"(*a), [d] "=m"(*d)
		: [b] "r"(b), [c] "r"(c)
		: "xr1", "xr2"
	);
}

static uint32_t q16sat(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"q16sat $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

#define GEN_Q8ADD_insn(aptn2) \
static uint32_t q8add##aptn2(uint32_t l, uint32_t r) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32i2m $xr1, %[l]\n" \
		"s32i2m $xr2, %[r]\n" \
		"q8add $xr1, $xr1, $xr2, %[ptn]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret) \
		: [l] "r"(l), [r] "r"(r), [ptn] "i"(aptn2) \
		: "xr1", "xr2" \
	); \
	return ret; \
}

GEN_Q8ADD_insn(0)
GEN_Q8ADD_insn(1)
GEN_Q8ADD_insn(2)
GEN_Q8ADD_insn(3)

typedef uint32_t (*q8add_proto_t)(uint32_t b, uint32_t c);
static uint32_t q8add(uint32_t b, uint32_t c, int aptn2)
{
	const q8add_proto_t fns[] = {
		q8add0, q8add1, q8add2, q8add3,
	};
	return fns[aptn2](b, c);
}

GEN_DQxxAxx_insn(q8adde, 0)
GEN_DQxxAxx_insn(q8adde, 1)
GEN_DQxxAxx_insn(q8adde, 2)
GEN_DQxxAxx_insn(q8adde, 3)

static void q8adde(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		_q8adde0, _q8adde1, _q8adde2, _q8adde3,
	};
	fns[aptn2](a, d, b, c);
}

GEN_DQxxAxx_insn(q8acce, 0)
GEN_DQxxAxx_insn(q8acce, 1)
GEN_DQxxAxx_insn(q8acce, 2)
GEN_DQxxAxx_insn(q8acce, 3)

static void q8acce(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, int aptn2)
{
	const d32add_proto_t fns[] = {
		_q8acce0, _q8acce1, _q8acce2, _q8acce3,
	};
	fns[aptn2](a, d, b, c);
}

static uint32_t q8abd(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"q8abd $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static void q8sad(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c)
{
	asm volatile (
		"s32i2m $xr1, %[b]\n"
		"s32i2m $xr2, %[c]\n"
		"s32ldd $xr3, %U[d]\n"
		"q8sad $xr1, $xr3, $xr1, $xr2\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr3, %U[d]\n"
		: [a] "=m"(*a), [d] "+m"(*d)
		: [b] "r"(b), [c] "r"(c)
		: "xr1", "xr2", "xr3"
	);
}

static uint32_t q8avg(uint32_t l, uint32_t r)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"q8avg $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static uint32_t q8avgr(uint32_t l, uint32_t r)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"q8avgr $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static uint32_t d8sum(uint32_t x, uint32_t y)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[x]\n"
		"s32i2m $xr2, %[y]\n"
		"d8sum  $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [x] "r"(x), [y] "r"(y)
		: "xr1", "xr2"
	);
	return ret;
}


static uint32_t d8sumc(uint32_t x, uint32_t y)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[x]\n"
		"s32i2m $xr2, %[y]\n"
		"d8sumc $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [x] "r"(x), [y] "r"(y)
		: "xr1", "xr2"
	);
	return ret;
}

static int32_t sign(int32_t val)
{
	if (val < 0)
		return -1;
	if (val > 0)
		return 1;
	return 0;
}

static uint32_t absu(uint32_t val1, uint32_t val2)
{
	if (val1 < val2)
		return val2 - val1;
	return val1 - val2;
}

void test_arithmetics(int num_passes)
{
	// test D32ADD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, cr, b, c;
			uint32_t expected_a = 0, expected_d = 0, expected_cr;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			if (aptn2 & 2) {
				expected_a = b - c;
				expected_cr = (b < c) ? 0 : 0x80000000;
			} else {
				expected_a = b + c;
				expected_cr = (b < expected_a) ? 0 : 0x80000000;
			}
			if (aptn2 & 1) {
				expected_d = b - c;
				expected_cr |= (b < c) ? 0 : 0x40000000;
			} else {
				expected_d = b + c;
				expected_cr |= (b < expected_d) ? 0 : 0x40000000;
			}
			d32add(&got_a, &got_d, b, c, aptn2);
			cr = s32m2i(16);
			cr &= 0xc0000000;
			if (got_a != expected_a || got_d != expected_d || expected_cr != cr) {
				printf("D32ADD expected: %08x %08x %08x, got %08x %08x %08x\n",
					expected_a, expected_d, expected_cr, got_a, got_d, cr);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32ADD, pass);
	}

	// test D32ADDC
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a, expected_d;
			uint32_t carry = random32() & 0xc0000000;
			b = random32();
			c = random32();
			got_a = expected_a = random32();
			got_d = expected_d = random32();
			expected_a += b + (carry >> 31);
			expected_d += c + ((carry >> 30) & 1u);
			d32addc(&got_a, &got_d, b, c, carry);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32ADDC expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32ADDC, pass);
	}

	// test D32ACC
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a, expected_d;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			got_a = expected_a = random32();
			got_d = expected_d = random32();
			if (aptn2 & 2) {
				expected_a += (b - c);
			} else {
				expected_a += (b + c);
			}
			if (aptn2 & 1) {
				expected_d += (b - c);
			} else {
				expected_d += (b + c);
			}
			d32acc(&got_a, &got_d, b, c, aptn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32ACC expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32ACC, pass);
	}

	// test D32ACCM
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a, expected_d;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			got_a = expected_a = random32();
			got_d = expected_d = random32();
			if (aptn2 & 2) {
				expected_a -= (b + c);
			} else {
				expected_a += (b + c);
			}
			if (aptn2 & 1) {
				expected_d -= (b - c);
			} else {
				expected_d += (b - c);
			}
			d32accm(&got_a, &got_d, b, c, aptn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32ACCM expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32ACCM, pass);
	}

	// test D32ASUM
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a, expected_d;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			got_a = expected_a = random32();
			got_d = expected_d = random32();
			if (aptn2 & 2) {
				expected_a -= b;
			} else {
				expected_a += b;
			}
			if (aptn2 & 1) {
				expected_d -= c;
			} else {
				expected_d += c;
			}
			d32asum(&got_a, &got_d, b, c, aptn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32ASUM expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32ASUM, pass);
	}

	// test Q16ADD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a = 0, expected_d = 0;
			uint16_t lop1 = 0, lop2 = 0, rop1 = 0, rop2 = 0;
			int aptn2 = random() % 4;
			int optn2 = random() % 4;
			b = random32();
			c = random32();
			switch (optn2) {
			case 0:
				lop1 = b >> 16; lop2 = c >> 16;
				rop1 = b; rop2 = c;
				break;
			case 1:
				lop1 = b; lop2 = c >> 16;
				rop1 = b; rop2 = c;
				break;
			case 2:
				lop1 = b >> 16; lop2 = c >> 16;
				rop1 = b >> 16; rop2 = c;
				break;
			case 3:
				lop1 = b; lop2 = c >> 16;
				rop1 = b >> 16; rop2 = c;
				break;
			}
			switch (aptn2) {
			case 0:
				expected_a = expected_d = ((lop1 + lop2) << 16) | ((rop1 + rop2) & 0xffff);
				break;
			case 1:
				expected_a = ((lop1 + lop2) << 16) | ((rop1 + rop2) & 0xffff);
				expected_d = ((lop1 - lop2) << 16) | ((rop1 - rop2) & 0xffff);
				break;
			case 2:
				expected_a = ((lop1 - lop2) << 16) | ((rop1 - rop2) & 0xffff);
				expected_d = ((lop1 + lop2) << 16) | ((rop1 + rop2) & 0xffff);
				break;
			case 3:
				expected_a = expected_d = ((lop1 - lop2) << 16) | ((rop1 - rop2) & 0xffff);
				break;
			}
			q16add(&got_a, &got_d, b, c, aptn2, optn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16ADD expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16ADD, pass);
	}

	// test Q16ACC
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a, expected_d;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			got_a = expected_a = random32();
			got_d = expected_d = random32();
			uint16_t *words_b = (uint16_t*)&b;
			uint16_t *words_c = (uint16_t*)&c;
			uint16_t *words_a = (uint16_t*)&expected_a;
			uint16_t *words_d = (uint16_t*)&expected_d;
			switch (aptn2) {
			case 0:
				words_a[0] += words_b[0] + words_c[0];
				words_a[1] += words_b[1] + words_c[1];
				words_d[0] += words_b[0] + words_c[0];
				words_d[1] += words_b[1] + words_c[1];
				break;
			case 1:
				words_a[0] += words_b[0] + words_c[0];
				words_a[1] += words_b[1] + words_c[1];
				words_d[0] += words_b[0] - words_c[0];
				words_d[1] += words_b[1] - words_c[1];
				break;
			case 2:
				words_a[0] += words_b[0] - words_c[0];
				words_a[1] += words_b[1] - words_c[1];
				words_d[0] += words_b[0] + words_c[0];
				words_d[1] += words_b[1] + words_c[1];
				break;
			case 3:
				words_a[0] += words_b[0] - words_c[0];
				words_a[1] += words_b[1] - words_c[1];
				words_d[0] += words_b[0] - words_c[0];
				words_d[1] += words_b[1] - words_c[1];
				break;
			}
			q16acc(&got_a, &got_d, b, c, aptn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16ACC expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16ACC, pass);
	}

	// test Q16ACCM
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a, expected_d;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			got_a = expected_a = random32();
			got_d = expected_d = random32();
			uint16_t *words_b = (uint16_t*)&b;
			uint16_t *words_c = (uint16_t*)&c;
			uint16_t *words_a = (uint16_t*)&expected_a;
			uint16_t *words_d = (uint16_t*)&expected_d;
			switch (aptn2) {
			case 0:
				words_a[0] += words_b[0];
				words_a[1] += words_b[1];
				words_d[0] += words_c[0];
				words_d[1] += words_c[1];
				break;
			case 1:
				words_a[0] += words_b[0];
				words_a[1] += words_b[1];
				words_d[0] -= words_c[0];
				words_d[1] -= words_c[1];
				break;
			case 2:
				words_a[0] -= words_b[0];
				words_a[1] -= words_b[1];
				words_d[0] += words_c[0];
				words_d[1] += words_c[1];
				break;
			case 3:
				words_a[0] -= words_b[0];
				words_a[1] -= words_b[1];
				words_d[0] -= words_c[0];
				words_d[1] -= words_c[1];
				break;
			}
			q16accm(&got_a, &got_d, b, c, aptn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16ACCM expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16ACCM, pass);
	}

	// test D16ASUM
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a, expected_d;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			got_a = expected_a = random32();
			got_d = expected_d = random32();
			uint16_t *words_b = (uint16_t*)&b;
			uint16_t *words_c = (uint16_t*)&c;
			switch (aptn2) {
			case 0:
				expected_a += (int32_t)(int16_t)words_b[0] + (int32_t)(int16_t)words_b[1];
				expected_d += (int32_t)(int16_t)words_c[0] + (int32_t)(int16_t)words_c[1];
				break;
			case 1:
				expected_a += (int32_t)(int16_t)words_b[0] + (int32_t)(int16_t)words_b[1];
				expected_d -= (int32_t)(int16_t)words_c[0] + (int32_t)(int16_t)words_c[1];
				break;
			case 2:
				expected_a -= (int32_t)(int16_t)words_b[0] + (int32_t)(int16_t)words_b[1];
				expected_d += (int32_t)(int16_t)words_c[0] + (int32_t)(int16_t)words_c[1];
				break;
			case 3:
				expected_a -= (int32_t)(int16_t)words_b[0] + (int32_t)(int16_t)words_b[1];
				expected_d -= (int32_t)(int16_t)words_c[0] + (int32_t)(int16_t)words_c[1];
				break;
			}
			d16asum(&got_a, &got_d, b, c, aptn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D16ASUM expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16ASUM, pass);
	}

	// test D16AVG
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t l = random32();
			uint32_t r = random32();
			int16_t l_hi, r_hi, l_lo, r_lo;
			l_hi = l >> 16; r_hi = r >> 16; l_lo = l; r_lo = r;
			uint32_t expected = (((int32_t)l_hi + r_hi) >> 1) << 16;
			expected |= (((int32_t)l_lo + r_lo) >> 1) & 0xffff;
			uint32_t got = d16avg(l, r);
			if (got != expected) {
				printf("D16AVG expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16AVG, pass);
	}

	// test D16AVGR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t l = random32();
			uint32_t r = random32();
			int16_t l_hi, r_hi, l_lo, r_lo;
			l_hi = l >> 16; r_hi = r >> 16; l_lo = l; r_lo = r;
			uint32_t expected = (((int32_t)l_hi + r_hi + 1) >> 1) << 16;
			expected |= (((int32_t)l_lo + r_lo + 1) >> 1) & 0xffff;
			uint32_t got = d16avgr(l, r);
			if (got != expected) {
				printf("D16AVGR expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16AVGR, pass);
	}

	// test Q16SCOP
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t got_a, b, c, got_d, expected_a = 0, expected_d = 0;
			b = random32();
			c = random32();
			expected_a |= (sign((int32_t)(b & 0xffff0000))) << 16;
			expected_a |= (sign((int32_t)(b << 16))) & 0xffff;
			expected_d |= (sign((int32_t)(c & 0xffff0000))) << 16;
			expected_d |= (sign((int32_t)(c << 16))) & 0xffff;
			q16scop(&got_a, &got_d, b, c);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16SCOP expected: %08x %08x got: %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16SCOP, pass);
	}

	// test Q16SAT
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t got, l, r, expected = 0;
			l = random32();
			r = random32();
			int16_t l_hi,r_hi,l_lo,r_lo;
			l_hi = l >> 16; r_hi = r >> 16;
			l_lo = l; r_lo = r;
			expected |= (l_hi < 0 ? 0 : (l_hi > 255 ? 255 : l_hi)) << 24;
			expected |= (l_lo < 0 ? 0 : (l_lo > 255 ? 255 : l_lo)) << 16;
			expected |= (r_hi < 0 ? 0 : (r_hi > 255 ? 255 : r_hi)) << 8;
			expected |= (r_lo < 0 ? 0 : (r_lo > 255 ? 255 : r_lo)) << 0;
			got = q16sat(l, r);
			if (got != expected) {
				printf("Q16SAT expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16SAT, pass);
	}

	// test Q8ADD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t l = random32();
			uint32_t r = random32();
			int aptn2 = random() % 4;
			uint8_t l_0, r_0, l_1, r_1, l_2, r_2, l_3, r_3;
			l_3 = l >> 24; r_3 = r >> 24; l_0 = l; r_0 = r;
			l_2 = l >> 16; r_2 = r >> 16; l_1 = l >> 8; r_1 = r >> 8;
			uint32_t expected = 0;
			expected  = (((aptn2 & 2) ? l_3 - r_3 : l_3 + r_3) & 0xff) << 24;
			expected |= (((aptn2 & 2) ? l_2 - r_2 : l_2 + r_2) & 0xff) << 16;
			expected |= (((aptn2 & 1) ? l_1 - r_1 : l_1 + r_1) & 0xff) << 8;
			expected |= (((aptn2 & 1) ? l_0 - r_0 : l_0 + r_0) & 0xff) << 0;
			uint32_t got = q8add(l, r, aptn2);
			if (got != expected) {
				printf("Q8ADD expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8ADD, pass);
	}

	// test Q8ADDE
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, expected_a = 0, expected_d = 0;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			uint8_t *bytes_b = (uint8_t*)&b;
			uint8_t *bytes_c = (uint8_t*)&c;
			switch (aptn2) {
			case 0:
				expected_a |= ((uint32_t)bytes_b[3] + bytes_c[3]) << 16;
				expected_a |= ((uint16_t)bytes_b[2] + bytes_c[2]) & 0xffff;
				expected_d |= ((uint32_t)bytes_b[1] + bytes_c[1]) << 16;
				expected_d |= ((uint16_t)bytes_b[0] + bytes_c[0]) & 0xffff;
				break;
			case 1:
				expected_a |= ((uint32_t)bytes_b[3] + bytes_c[3]) << 16;
				expected_a |= ((uint16_t)bytes_b[2] + bytes_c[2]) & 0xffff;
				expected_d |= ((uint32_t)bytes_b[1] - bytes_c[1]) << 16;
				expected_d |= ((uint16_t)bytes_b[0] - bytes_c[0]) & 0xffff;
				break;
			case 2:
				expected_a |= ((uint32_t)bytes_b[3] - bytes_c[3]) << 16;
				expected_a |= ((uint16_t)bytes_b[2] - bytes_c[2]) & 0xffff;
				expected_d |= ((uint32_t)bytes_b[1] + bytes_c[1]) << 16;
				expected_d |= ((uint16_t)bytes_b[0] + bytes_c[0]) & 0xffff;
				break;
			case 3:
				expected_a |= ((uint32_t)bytes_b[3] - bytes_c[3]) << 16;
				expected_a |= ((uint16_t)bytes_b[2] - bytes_c[2]) & 0xffff;
				expected_d |= ((uint32_t)bytes_b[1] - bytes_c[1]) << 16;
				expected_d |= ((uint16_t)bytes_b[0] - bytes_c[0]) & 0xffff;
				break;
			}
			q8adde(&got_a, &got_d, b, c, aptn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q8ADDE expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8ADDE, pass);
	}

	// test Q8ACCE
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t got_a, got_d, b, c, acc_a, acc_d, expected_a = 0, expected_d = 0;
			int aptn2 = random() % 4;
			b = random32();
			c = random32();
			uint8_t *bytes_b = (uint8_t*)&b;
			uint8_t *bytes_c = (uint8_t*)&c;
			got_a = acc_a = random32();
			got_d = acc_d = random32();
			switch (aptn2) {
			case 0:
				expected_a |= ((acc_a >> 16)    + bytes_b[3] + bytes_c[3]) << 16;
				expected_a |= ((acc_a & 0xffff) + bytes_b[2] + bytes_c[2]) & 0xffff;
				expected_d |= ((acc_d >> 16)    + bytes_b[1] + bytes_c[1]) << 16;
				expected_d |= ((acc_d & 0xffff) + bytes_b[0] + bytes_c[0]) & 0xffff;
				break;
			case 1:
				expected_a |= ((acc_a >> 16)    + bytes_b[3] + bytes_c[3]) << 16;
				expected_a |= ((acc_a & 0xffff) + bytes_b[2] + bytes_c[2]) & 0xffff;
				expected_d |= ((acc_d >> 16)    + bytes_b[1] - bytes_c[1]) << 16;
				expected_d |= ((acc_d & 0xffff) + bytes_b[0] - bytes_c[0]) & 0xffff;
				break;
			case 2:
				expected_a |= ((acc_a >> 16)    + bytes_b[3] - bytes_c[3]) << 16;
				expected_a |= ((acc_a & 0xffff) + bytes_b[2] - bytes_c[2]) & 0xffff;
				expected_d |= ((acc_d >> 16)    + bytes_b[1] + bytes_c[1]) << 16;
				expected_d |= ((acc_d & 0xffff) + bytes_b[0] + bytes_c[0]) & 0xffff;
				break;
			case 3:
				expected_a |= ((acc_a >> 16)    + bytes_b[3] - bytes_c[3]) << 16;
				expected_a |= ((acc_a & 0xffff) + bytes_b[2] - bytes_c[2]) & 0xffff;
				expected_d |= ((acc_d >> 16)    + bytes_b[1] - bytes_c[1]) << 16;
				expected_d |= ((acc_d & 0xffff) + bytes_b[0] - bytes_c[0]) & 0xffff;
				break;
			}
			q8acce(&got_a, &got_d, b, c, aptn2);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q8ACCE expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8ACCE, pass);
	}

	// test Q8ABD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected = 0;
			uint8_t l_0, r_0, l_1, r_1, l_2, r_2, l_3, r_3;
			l_3 = l >> 24; r_3 = r >> 24; l_0 = l; r_0 = r;
			l_2 = l >> 16; r_2 = r >> 16; l_1 = l >> 8; r_1 = r >> 8;
			expected |= abs((int32_t)l_3 - (int32_t)r_3) << 24;
			expected |= (abs((int32_t)l_2 - (int32_t)r_2) & 0xff) << 16;
			expected |= (abs((int32_t)l_1 - (int32_t)r_1) & 0xff) << 8;
			expected |= (abs((int32_t)l_0 - (int32_t)r_0) & 0xff) << 0;
			uint32_t got = q8abd(l, r);
			if (got != expected) {
				printf("Q8ABD expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8ABD, pass);
	}

	// test Q8SAD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t got_a, got_d, b, c, expected_a = 0, expected_d;
			b = random32();
			c = random32();
			got_d = random32();
			expected_a += absu(b & 0xff, c & 0xff);
			expected_a += absu((b >> 8) & 0xff, (c >> 8) & 0xff);
			expected_a += absu((b >> 16) & 0xff, (c >> 16) & 0xff);
			expected_a += absu(b >> 24, c >> 24);
			expected_d = got_d + expected_a;
			q8sad(&got_a, &got_d, b, c);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q8SAD expected: %08x %08x, got %08x %08x\n",
					expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8SAD, pass);
	}

	// test Q8AVG
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t l = random32();
			uint32_t r = random32();
			uint8_t l_0, r_0, l_1, r_1, l_2, r_2, l_3, r_3;
			l_3 = l >> 24; r_3 = r >> 24; l_0 = l; r_0 = r;
			l_2 = l >> 16; r_2 = r >> 16; l_1 = l >> 8; r_1 = r >> 8;
			uint32_t expected = (((uint32_t)l_3 + r_3) >> 1) << 24;
			expected |= ((((uint32_t)l_2 + r_2) >> 1) & 0xff) << 16;
			expected |= ((((uint32_t)l_1 + r_1) >> 1) & 0xff) << 8;
			expected |= ((((uint32_t)l_0 + r_0) >> 1) & 0xff);
			uint32_t got = q8avg(l, r);
			if (got != expected) {
				printf("Q8AVG expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8AVG, pass);
	}

	// test Q8AVGR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t l = random32();
			uint32_t r = random32();
			uint8_t l_0, r_0, l_1, r_1, l_2, r_2, l_3, r_3;
			l_3 = l >> 24; r_3 = r >> 24; l_0 = l; r_0 = r;
			l_2 = l >> 16; r_2 = r >> 16; l_1 = l >> 8; r_1 = r >> 8;
			uint32_t expected = (((uint32_t)l_3 + r_3 + 1) >> 1) << 24;
			expected |= ((((uint32_t)l_2 + r_2 + 1) >> 1) & 0xff) << 16;
			expected |= ((((uint32_t)l_1 + r_1 + 1) >> 1) & 0xff) << 8;
			expected |= ((((uint32_t)l_0 + r_0 + 1) >> 1) & 0xff);
			uint32_t got = q8avgr(l, r);
			if (got != expected) {
				printf("Q8AVGR expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8AVGR, pass);
	}

	// test D8SUM
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t b = random32();
			uint32_t c = random32();
			uint8_t *bytes_b = (uint8_t*)&b;
			uint8_t *bytes_c = (uint8_t*)&c;
			uint32_t expected = 0;
			uint16_t *words_exp = (uint16_t*)&expected;
			words_exp[1] = (uint16_t)bytes_b[0] + bytes_b[1] + bytes_b[2] + bytes_b[3];
			words_exp[0] = (uint16_t)bytes_c[0] + bytes_c[1] + bytes_c[2] + bytes_c[3];
			uint32_t got = d8sum(b, c);
			if (got != expected) {
				printf("D8SUM expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D8SUM, pass);
	}

	// test D8SUMC
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t b = random32();
			uint32_t c = random32();
			uint8_t *bytes_b = (uint8_t*)&b;
			uint8_t *bytes_c = (uint8_t*)&c;
			uint32_t expected = 0;
			uint16_t *words_exp = (uint16_t*)&expected;
			words_exp[1] = (uint16_t)bytes_b[0] + bytes_b[1] + bytes_b[2] + bytes_b[3] + 2;
			words_exp[0] = (uint16_t)bytes_c[0] + bytes_c[1] + bytes_c[2] + bytes_c[3] + 2;
			uint32_t got = d8sumc(b, c);
			if (got != expected) {
				printf("D8SUMC expected: %08x, got %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D8SUMC, pass);
	}
}
