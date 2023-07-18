#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

#define GEN_S32EXTR_insn(bits5) \
static uint32_t s32extr##bits5(uint32_t hi, uint32_t lo, unsigned shift) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32i2m $xr1, %[hi]\n" \
		"s32i2m $xr2, %[lo]\n" \
		"s32extr $xr1, $xr2, %[shift], %[bits]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret) \
		: [hi] "r"(hi), [lo] "r"(lo), [shift] "r"(shift), [bits] "i"(bits5) \
		: "xr1", "xr2" \
	); \
	return ret; \
}

GEN_S32EXTR_insn(0)
GEN_S32EXTR_insn(1)
GEN_S32EXTR_insn(2)
GEN_S32EXTR_insn(3)
GEN_S32EXTR_insn(4)
GEN_S32EXTR_insn(5)
GEN_S32EXTR_insn(6)
GEN_S32EXTR_insn(7)
GEN_S32EXTR_insn(8)
GEN_S32EXTR_insn(9)
GEN_S32EXTR_insn(10)
GEN_S32EXTR_insn(11)
GEN_S32EXTR_insn(12)
GEN_S32EXTR_insn(13)
GEN_S32EXTR_insn(14)
GEN_S32EXTR_insn(15)
GEN_S32EXTR_insn(16)
GEN_S32EXTR_insn(17)
GEN_S32EXTR_insn(18)
GEN_S32EXTR_insn(19)
GEN_S32EXTR_insn(20)
GEN_S32EXTR_insn(21)
GEN_S32EXTR_insn(22)
GEN_S32EXTR_insn(23)
GEN_S32EXTR_insn(24)
GEN_S32EXTR_insn(25)
GEN_S32EXTR_insn(26)
GEN_S32EXTR_insn(27)
GEN_S32EXTR_insn(28)
GEN_S32EXTR_insn(29)
GEN_S32EXTR_insn(30)
GEN_S32EXTR_insn(31)

typedef uint32_t (*s32extr_proto_t)(uint32_t hi, uint32_t lo, unsigned shift);

static uint32_t s32extr(uint32_t hi, uint32_t lo, unsigned shift, unsigned bits5)
{
	const s32extr_proto_t fns[] = {
		s32extr0,  s32extr1,  s32extr2,  s32extr3,
		s32extr4,  s32extr5,  s32extr6,  s32extr7,
		s32extr8,  s32extr9,  s32extr10, s32extr11,
		s32extr12, s32extr13, s32extr14, s32extr15,
		s32extr16, s32extr17, s32extr18, s32extr19,
		s32extr20, s32extr21, s32extr22, s32extr23,
		s32extr24, s32extr25, s32extr26, s32extr27,
		s32extr28, s32extr29, s32extr30, s32extr31,
	};

	return fns[bits5](hi, lo, shift);
}

static uint32_t s32extrv(uint32_t hi, uint32_t lo, unsigned shift, unsigned bits5)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[hi]\n"
		"s32i2m $xr2, %[lo]\n"
		"s32extrv $xr1, $xr2, %[shift], %[bits]\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [hi] "r"(hi), [lo] "r"(lo), [shift] "r"(shift), [bits] "r"(bits5)
		: "xr1", "xr2"
	);
	return ret;
}

#define GEN_D32SARL_insn(sft4) \
static uint32_t d32sarl##sft4(uint32_t b, uint32_t c) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"d32sarl $xr1, $xr1, $xr2, %[sft]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret) \
		: [b] "r"(b), [c] "r"(c), [sft] "I"(sft4) \
		: "xr1", "xr2" \
	); \
	return ret; \
}

GEN_D32SARL_insn(0)
GEN_D32SARL_insn(1)
GEN_D32SARL_insn(2)
GEN_D32SARL_insn(3)
GEN_D32SARL_insn(4)
GEN_D32SARL_insn(5)
GEN_D32SARL_insn(6)
GEN_D32SARL_insn(7)
GEN_D32SARL_insn(8)
GEN_D32SARL_insn(9)
GEN_D32SARL_insn(10)
GEN_D32SARL_insn(11)
GEN_D32SARL_insn(12)
GEN_D32SARL_insn(13)
GEN_D32SARL_insn(14)
GEN_D32SARL_insn(15)

typedef uint32_t (*d32sarl_proto_t)(uint32_t b, uint32_t c);

static uint32_t d32sarl(uint32_t b, uint32_t c, unsigned sft4)
{
	const d32sarl_proto_t fns[] = {
		d32sarl0,  d32sarl1,  d32sarl2,  d32sarl3,
		d32sarl4,  d32sarl5,  d32sarl6,  d32sarl7,
		d32sarl8,  d32sarl9,  d32sarl10, d32sarl11,
		d32sarl12, d32sarl13, d32sarl14, d32sarl15,
	};
	return fns[sft4](b, c);
}


static uint32_t d32sarw(uint32_t b, uint32_t c, unsigned shift)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[b]\n"
		"s32i2m $xr2, %[c]\n"
		"d32sarw $xr1, $xr1, $xr2, %[shift]\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [b] "r"(b), [c] "r"(c), [shift] "r"(shift)
		: "xr1", "xr2"
	);
	return ret;
}


#define GEN_S32ALNI_insn(optn3) \
static uint32_t s32alni##optn3(uint32_t b, uint32_t c) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"s32alni $xr1, $xr1, $xr2, %[ptn]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret) \
		: [b] "r"(b), [c] "r"(c), [ptn] "I"(optn3) \
		: "xr1", "xr2" \
	); \
	return ret; \
}

GEN_S32ALNI_insn(0)
GEN_S32ALNI_insn(1)
GEN_S32ALNI_insn(2)
GEN_S32ALNI_insn(3)
GEN_S32ALNI_insn(4)

typedef uint32_t (*s32alni_proto_t)(uint32_t b, uint32_t c);
static uint32_t s32alni(uint32_t b, uint32_t c, unsigned optn3)
{
	const d32sarl_proto_t fns[] = {
		s32alni0,  s32alni1,  s32alni2,  s32alni3,
		s32alni4,
	};
	return fns[optn3](b, c);
}

static uint32_t s32aln(uint32_t b, uint32_t c, unsigned optn3)
{
	uint32_t ret;
	asm volatile (
		"s32i2m $xr1, %[b]\n"
		"s32i2m $xr2, %[c]\n"
		"s32aln $xr1, $xr1, $xr2, %[ptn]\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [b] "r"(b), [c] "r"(c), [ptn] "r"(optn3)
		: "xr1", "xr2"
	);
	return ret;
}


#define GEN_D32Sxx_insn(fn,sft4) \
static void d32s##fn##sft4(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"d32s" #fn " $xr1, $xr2, $xr1, $xr2, %[sft]\n" \
		"s32std $xr1, %U[a]\n" \
		"s32std $xr2, %U[d]\n" \
		: [a] "=m"(*a), [d] "=m"(*d) \
		: [b] "r"(b), [c] "r"(c), [sft] "I"(sft4) \
		: "xr1", "xr2" \
	); \
}

GEN_D32Sxx_insn(ll,0)
GEN_D32Sxx_insn(ll,1)
GEN_D32Sxx_insn(ll,2)
GEN_D32Sxx_insn(ll,3)
GEN_D32Sxx_insn(ll,4)
GEN_D32Sxx_insn(ll,5)
GEN_D32Sxx_insn(ll,6)
GEN_D32Sxx_insn(ll,7)
GEN_D32Sxx_insn(ll,8)
GEN_D32Sxx_insn(ll,9)
GEN_D32Sxx_insn(ll,10)
GEN_D32Sxx_insn(ll,11)
GEN_D32Sxx_insn(ll,12)
GEN_D32Sxx_insn(ll,13)
GEN_D32Sxx_insn(ll,14)
GEN_D32Sxx_insn(ll,15)

GEN_D32Sxx_insn(lr,0)
GEN_D32Sxx_insn(lr,1)
GEN_D32Sxx_insn(lr,2)
GEN_D32Sxx_insn(lr,3)
GEN_D32Sxx_insn(lr,4)
GEN_D32Sxx_insn(lr,5)
GEN_D32Sxx_insn(lr,6)
GEN_D32Sxx_insn(lr,7)
GEN_D32Sxx_insn(lr,8)
GEN_D32Sxx_insn(lr,9)
GEN_D32Sxx_insn(lr,10)
GEN_D32Sxx_insn(lr,11)
GEN_D32Sxx_insn(lr,12)
GEN_D32Sxx_insn(lr,13)
GEN_D32Sxx_insn(lr,14)
GEN_D32Sxx_insn(lr,15)

GEN_D32Sxx_insn(ar,0)
GEN_D32Sxx_insn(ar,1)
GEN_D32Sxx_insn(ar,2)
GEN_D32Sxx_insn(ar,3)
GEN_D32Sxx_insn(ar,4)
GEN_D32Sxx_insn(ar,5)
GEN_D32Sxx_insn(ar,6)
GEN_D32Sxx_insn(ar,7)
GEN_D32Sxx_insn(ar,8)
GEN_D32Sxx_insn(ar,9)
GEN_D32Sxx_insn(ar,10)
GEN_D32Sxx_insn(ar,11)
GEN_D32Sxx_insn(ar,12)
GEN_D32Sxx_insn(ar,13)
GEN_D32Sxx_insn(ar,14)
GEN_D32Sxx_insn(ar,15)

typedef void (*d32sxx_proto_t)(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c);

static void d32sll(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, unsigned sft4)
{
	const d32sxx_proto_t fns[] = {
		d32sll0,  d32sll1,  d32sll2,  d32sll3,
		d32sll4,  d32sll5,  d32sll6,  d32sll7,
		d32sll8,  d32sll9,  d32sll10, d32sll11,
		d32sll12, d32sll13, d32sll14, d32sll15,
	};
	fns[sft4](a, d, b, c);
}

static void d32slr(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, unsigned sft4)
{
	const d32sxx_proto_t fns[] = {
		d32slr0,  d32slr1,  d32slr2,  d32slr3,
		d32slr4,  d32slr5,  d32slr6,  d32slr7,
		d32slr8,  d32slr9,  d32slr10, d32slr11,
		d32slr12, d32slr13, d32slr14, d32slr15,
	};
	fns[sft4](a, d, b, c);
}

static void d32sar(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, unsigned sft4)
{
	const d32sxx_proto_t fns[] = {
		d32sar0,  d32sar1,  d32sar2,  d32sar3,
		d32sar4,  d32sar5,  d32sar6,  d32sar7,
		d32sar8,  d32sar9,  d32sar10, d32sar11,
		d32sar12, d32sar13, d32sar14, d32sar15,
	};
	fns[sft4](a, d, b, c);
}



#define GEN_Q16Sxx_insn(fn, sft4) \
static void q16s##fn##sft4(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"q16s" #fn " $xr1, $xr2, $xr1, $xr2, %[sft]\n" \
		"s32std $xr1, %U[a]\n" \
		"s32std $xr2, %U[d]\n" \
		: [a] "=m"(*a), [d] "=m"(*d) \
		: [b] "r"(b), [c] "r"(c), [sft] "I"(sft4) \
		: "xr1", "xr2" \
	); \
}

GEN_Q16Sxx_insn(ll,0)
GEN_Q16Sxx_insn(ll,1)
GEN_Q16Sxx_insn(ll,2)
GEN_Q16Sxx_insn(ll,3)
GEN_Q16Sxx_insn(ll,4)
GEN_Q16Sxx_insn(ll,5)
GEN_Q16Sxx_insn(ll,6)
GEN_Q16Sxx_insn(ll,7)
GEN_Q16Sxx_insn(ll,8)
GEN_Q16Sxx_insn(ll,9)
GEN_Q16Sxx_insn(ll,10)
GEN_Q16Sxx_insn(ll,11)
GEN_Q16Sxx_insn(ll,12)
GEN_Q16Sxx_insn(ll,13)
GEN_Q16Sxx_insn(ll,14)
GEN_Q16Sxx_insn(ll,15)

GEN_Q16Sxx_insn(lr,0)
GEN_Q16Sxx_insn(lr,1)
GEN_Q16Sxx_insn(lr,2)
GEN_Q16Sxx_insn(lr,3)
GEN_Q16Sxx_insn(lr,4)
GEN_Q16Sxx_insn(lr,5)
GEN_Q16Sxx_insn(lr,6)
GEN_Q16Sxx_insn(lr,7)
GEN_Q16Sxx_insn(lr,8)
GEN_Q16Sxx_insn(lr,9)
GEN_Q16Sxx_insn(lr,10)
GEN_Q16Sxx_insn(lr,11)
GEN_Q16Sxx_insn(lr,12)
GEN_Q16Sxx_insn(lr,13)
GEN_Q16Sxx_insn(lr,14)
GEN_Q16Sxx_insn(lr,15)

GEN_Q16Sxx_insn(ar,0)
GEN_Q16Sxx_insn(ar,1)
GEN_Q16Sxx_insn(ar,2)
GEN_Q16Sxx_insn(ar,3)
GEN_Q16Sxx_insn(ar,4)
GEN_Q16Sxx_insn(ar,5)
GEN_Q16Sxx_insn(ar,6)
GEN_Q16Sxx_insn(ar,7)
GEN_Q16Sxx_insn(ar,8)
GEN_Q16Sxx_insn(ar,9)
GEN_Q16Sxx_insn(ar,10)
GEN_Q16Sxx_insn(ar,11)
GEN_Q16Sxx_insn(ar,12)
GEN_Q16Sxx_insn(ar,13)
GEN_Q16Sxx_insn(ar,14)
GEN_Q16Sxx_insn(ar,15)

typedef void (*q16sxx_proto_t)(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c);

static void q16sll(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, unsigned sft4)
{
	const q16sxx_proto_t fns[] = {
		q16sll0,  q16sll1,  q16sll2,  q16sll3,
		q16sll4,  q16sll5,  q16sll6,  q16sll7,
		q16sll8,  q16sll9,  q16sll10, q16sll11,
		q16sll12, q16sll13, q16sll14, q16sll15,
	};
	fns[sft4](a, d, b, c);
}

static void q16slr(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, unsigned sft4)
{
	const q16sxx_proto_t fns[] = {
		q16slr0,  q16slr1,  q16slr2,  q16slr3,
		q16slr4,  q16slr5,  q16slr6,  q16slr7,
		q16slr8,  q16slr9,  q16slr10, q16slr11,
		q16slr12, q16slr13, q16slr14, q16slr15,
	};
	fns[sft4](a, d, b, c);
}

static void q16sar(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, unsigned sft4)
{
	const q16sxx_proto_t fns[] = {
		q16sar0,  q16sar1,  q16sar2,  q16sar3,
		q16sar4,  q16sar5,  q16sar6,  q16sar7,
		q16sar8,  q16sar9,  q16sar10, q16sar11,
		q16sar12, q16sar13, q16sar14, q16sar15,
	};
	fns[sft4](a, d, b, c);
}


static void d32sllv(uint32_t *a, uint32_t *d, uint32_t sft)
{
	asm volatile (
		"s32ldd $xr1, %U[a]\n"
		"s32ldd $xr2, %U[d]\n"
		"d32sllv $xr1, $xr2, %[sft]\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr2, %U[d]\n"
		: [a] "+m"(*a), [d] "+m"(*d)
		: [sft] "r"(sft)
		: "xr1", "xr2"
	);
}

static void d32slrv(uint32_t *a, uint32_t *d, uint32_t sft)
{
	asm volatile (
		"s32ldd $xr1, %U[a]\n"
		"s32ldd $xr2, %U[d]\n"
		"d32slrv $xr1, $xr2, %[sft]\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr2, %U[d]\n"
		: [a] "+m"(*a), [d] "+m"(*d)
		: [sft] "r"(sft)
		: "xr1", "xr2"
	);
}

static void d32sarv(uint32_t *a, uint32_t *d, uint32_t sft)
{
	asm volatile (
		"s32ldd $xr1, %U[a]\n"
		"s32ldd $xr2, %U[d]\n"
		"d32sarv $xr1, $xr2, %[sft]\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr2, %U[d]\n"
		: [a] "+m"(*a), [d] "+m"(*d)
		: [sft] "r"(sft)
		: "xr1", "xr2"
	);
}

static void q16sllv(uint32_t *a, uint32_t *d, uint32_t sft)
{
	asm volatile (
		"s32ldd $xr1, %U[a]\n"
		"s32ldd $xr2, %U[d]\n"
		"q16sllv $xr1, $xr2, %[sft]\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr2, %U[d]\n"
		: [a] "+m"(*a), [d] "+m"(*d)
		: [sft] "r"(sft)
		: "xr1", "xr2"
	);
}

static void q16slrv(uint32_t *a, uint32_t *d, uint32_t sft)
{
	asm volatile (
		"s32ldd $xr1, %U[a]\n"
		"s32ldd $xr2, %U[d]\n"
		"q16slrv $xr1, $xr2, %[sft]\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr2, %U[d]\n"
		: [a] "+m"(*a), [d] "+m"(*d)
		: [sft] "r"(sft)
		: "xr1", "xr2"
	);
}

static void q16sarv(uint32_t *a, uint32_t *d, uint32_t sft)
{
	asm volatile (
		"s32ldd $xr1, %U[a]\n"
		"s32ldd $xr2, %U[d]\n"
		"q16sarv $xr1, $xr2, %[sft]\n"
		"s32std $xr1, %U[a]\n"
		"s32std $xr2, %U[d]\n"
		: [a] "+m"(*a), [d] "+m"(*d)
		: [sft] "r"(sft)
		: "xr1", "xr2"
	);
}

#define GEN_S32SFL_insn(optn2) \
static void s32sfl##optn2(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"s32sfl $xr1, $xr2, $xr1, $xr2, %[ptn]\n" \
		"s32std $xr1, %U[a]\n" \
		"s32std $xr2, %U[d]\n" \
		: [a] "=m"(*a), [d] "=m"(*d) \
		: [b] "r"(b), [c] "r"(c), [ptn] "I"(optn2) \
		: "xr1", "xr2" \
	); \
} \

GEN_S32SFL_insn(0)
GEN_S32SFL_insn(1)
GEN_S32SFL_insn(2)
GEN_S32SFL_insn(3)

typedef void (*s32sfl_proto_t)(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c);

static void s32sfl(uint32_t *a, uint32_t *d, uint32_t b, uint32_t c, unsigned optn2)
{
	const s32sfl_proto_t fns[] = {
		s32sfl0, s32sfl1, s32sfl2, s32sfl3
	};
	fns[optn2](a, d, b, c);
}


void test_shifts(int num_passes)
{
	// test S32EXTR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int bits = random() % 32;
			const int shift = random() % 32;
			uint64_t hilo = random64();
			uint32_t hi = hilo >> 32;
			uint32_t lo = hilo & 0xffffffff;
			uint32_t expected = (hilo >> ((32 - shift) + (32 - bits))) & ((1u << bits) - 1);
			uint32_t got = s32extr(hi, lo, shift, bits);
			if (expected != got) {
				printf("S32EXTR expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32EXTR, pass);
	}

	// test S32EXTRV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int bits = random() % 32;
			const int shift = random() % 32;
			uint64_t hilo = random64();
			uint32_t hi = hilo >> 32;
			uint32_t lo = hilo & 0xffffffff;
			uint32_t expected = (hilo >> ((32 - shift) + (32 - bits))) & ((1u << bits) - 1);
			uint32_t got = s32extrv(hi, lo, shift, bits);
			if (expected != got) {
				printf("S32EXTRV expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32EXTRV, pass);
	}

	// test D32SARL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected = ((b >> shift) << 16) | ((c >> shift) & 0xffff);
			uint32_t got = d32sarl(b, c, shift);
			if (expected != got) {
				printf("D32SARL expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32SARL, pass);
	}

	// test D32SARW
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected = ((b >> shift) << 16) | ((c >> shift) & 0xffff);
			uint32_t got = d32sarw(b, c, shift);
			if (expected != got) {
				printf("D32SARW expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32SARW, pass);
	}

	// test S32SALNI
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t optn3 = random() % 5;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected = 0;
			switch (optn3) {
			case 0:
				expected = b;
				break;
			case 1:
				expected = (b << 8) | (c >> 24);
				break;
			case 2:
				expected = (b << 16) | (c >> 16);
				break;
			case 3:
				expected = (b << 24) | (c >> 8);
				break;
			case 4:
				expected = c;
				break;
			}
			uint32_t got = s32alni(b, c, optn3);
			if (expected != got) {
				printf("S32SALNI expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32SALNI, pass);
	}

	// test S32SALN
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t optn3 = random() % 5;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected = 0;
			switch (optn3) {
			case 0:
				expected = b;
				break;
			case 1:
				expected = (b << 8) | (c >> 24);
				break;
			case 2:
				expected = (b << 16) | (c >> 16);
				break;
			case 3:
				expected = (b << 24) | (c >> 8);
				break;
			case 4:
				expected = c;
				break;
			}
			uint32_t got = s32aln(b, c, optn3);
			if (expected != got) {
				printf("S32SALN expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32SALN, pass);
	}

	// test D32SLL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = b << shift;
			uint32_t expected_d = c << shift;
			uint32_t got_a, got_d;
			d32sll(&got_a, &got_d, b, c, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32SLL expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32SLL, pass);
	}

	// test D32SLR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = b >> shift;
			uint32_t expected_d = c >> shift;
			uint32_t got_a, got_d;
			d32slr(&got_a, &got_d, b, c, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32SLR expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32SLR, pass);
	}

	// test D32SAR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = (int32_t)b >> shift;
			uint32_t expected_d = (int32_t)c >> shift;
			uint32_t got_a, got_d;
			d32sar(&got_a, &got_d, b, c, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32SAR expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32SAR, pass);
	}

	// test D32SLLV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = b << shift;
			uint32_t expected_d = c << shift;
			uint32_t got_a = b, got_d = c;
			d32sllv(&got_a, &got_d, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32SLLV expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32SLLV, pass);
	}

	// test D32SLRV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = b >> shift;
			uint32_t expected_d = c >> shift;
			uint32_t got_a = b, got_d = c;
			d32slrv(&got_a, &got_d, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32SLRV expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32SLRV, pass);
	}

	// test D32SARV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = (int32_t)b >> shift;
			uint32_t expected_d = (int32_t)c >> shift;
			uint32_t got_a = b, got_d = c;
			d32sarv(&got_a, &got_d, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("D32SARV expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D32SARV, pass);
	}

	// test Q16SLL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = (((b & 0xffff) << shift) & 0xffff) | ((b & 0xffff0000) << shift);
			uint32_t expected_d = (((c & 0xffff) << shift) & 0xffff) | ((c & 0xffff0000) << shift);
			uint32_t got_a, got_d;
			q16sll(&got_a, &got_d, b, c, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16SLL expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16SLL, pass);
	}

	// test Q16SLR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = ((b & 0xffff) >> shift) | (((b & 0xffff0000) >> shift) & 0xffff0000);
			uint32_t expected_d = ((c & 0xffff) >> shift) | (((c & 0xffff0000) >> shift) & 0xffff0000);
			uint32_t got_a, got_d;
			q16slr(&got_a, &got_d, b, c, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16SLR expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16SLR, pass);
	}

	// test Q16SAR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = (((int32_t)(int16_t)b >> shift) & 0xffff) | (((int32_t)b >> shift) & 0xffff0000);
			uint32_t expected_d = (((int32_t)(int16_t)c >> shift) & 0xffff) | (((int32_t)c >> shift) & 0xffff0000);
			uint32_t got_a, got_d;
			q16sar(&got_a, &got_d, b, c, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16SAR expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16SAR, pass);
	}

	// test Q16SLLV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = (((b & 0xffff) << shift) & 0xffff) | ((b & 0xffff0000) << shift);
			uint32_t expected_d = (((c & 0xffff) << shift) & 0xffff) | ((c & 0xffff0000) << shift);
			uint32_t got_a = b, got_d = c;
			q16sllv(&got_a, &got_d, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16SLLV expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16SLLV, pass);
	}

	// test Q16SLRV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = ((b & 0xffff) >> shift) | (((b & 0xffff0000) >> shift) & 0xffff0000);
			uint32_t expected_d = ((c & 0xffff) >> shift) | (((c & 0xffff0000) >> shift) & 0xffff0000);
			uint32_t got_a = b, got_d = c;
			q16slrv(&got_a, &got_d, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16SLRV expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16SLRV, pass);
	}

	// test Q16SARV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int shift = random() % 16;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = (((int32_t)(int16_t)b >> shift) & 0xffff) | (((int32_t)b >> shift) & 0xffff0000);
			uint32_t expected_d = (((int32_t)(int16_t)c >> shift) & 0xffff) | (((int32_t)c >> shift) & 0xffff0000);
			uint32_t got_a = b, got_d = c;
			q16sarv(&got_a, &got_d, shift);
			if (got_a != expected_a || got_d != expected_d) {
				printf("Q16SARV expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q16SARV, pass);
	}

	// test S32SFL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			const int op = random() % 4;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t expected_a = b & 0xff000000;
			uint32_t expected_d = c & 0x000000ff;
			uint32_t got_a, got_d;
			switch (op) {
			case 0:
				expected_a |= (c >>  8) & 0x00ff0000;
				expected_a |= (b >>  8) & 0x0000ff00;
				expected_a |= (c >> 16) & 0x000000ff;
				expected_d |= (b << 16) & 0xff000000;
				expected_d |= (c <<  8) & 0x00ff0000;
				expected_d |= (b <<  8) & 0x0000ff00;
				break;
			case 1:
				expected_a |= (b <<  8) & 0x00ff0000;
				expected_a |= (c >> 16) & 0x0000ff00;
				expected_a |= (c >>  8) & 0x000000ff;
				expected_d |= (b <<  8) & 0xff000000;
				expected_d |= (b << 16) & 0x00ff0000;
				expected_d |= (c >>  8) & 0x0000ff00;
				break;
			case 2:
				expected_a |= (c >>  8) & 0x00ff00ff;
				expected_a |= (b >>  0) & 0x0000ff00;
				expected_d |= (b <<  8) & 0xff00ff00;
				expected_d |= (c <<  0) & 0x00ff0000;
				break;
			case 3:
				expected_a |= (b <<  0) & 0x00ff0000;
				expected_a |= (c >> 16) & 0x0000ffff;
				expected_d |= (b << 16) & 0xffff0000;
				expected_d |= (c >>  0) & 0x0000ff00;
				break;
			}
			s32sfl(&got_a, &got_d, b, c, op);
			if (got_a != expected_a || got_d != expected_d) {
				printf("S32SFL expected: %08x %08x got: %08x %08x\n",
				       expected_a, expected_d, got_a, got_d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32SFL, pass);
	}
}

static uint32_t s32nor(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"s32nor $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static uint32_t s32and(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"s32and $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static uint32_t s32xor(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"s32xor $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}

static uint32_t s32or(uint32_t l, uint32_t r)
{
	uint32_t ret;

	asm volatile (
		"s32i2m $xr1, %[l]\n"
		"s32i2m $xr2, %[r]\n"
		"s32or $xr1, $xr1, $xr2\n"
		"s32m2i $xr1, %[ret]\n"
		: [ret] "=r"(ret)
		: [l] "r"(l), [r] "r"(r)
		: "xr1", "xr2"
	);
	return ret;
}


void test_bitwise(int num_passes)
{
	// test S32NOR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected = ~(l | r);
			uint32_t got = s32nor(l, r);
			if (expected != got) {
				printf("S32NOR expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32NOR, pass);
	}

	// test S32AND
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected = l & r;
			uint32_t got = s32and(l, r);
			if (expected != got) {
				printf("S32AND expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32AND, pass);
	}

	// test S32XOR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected = l ^ r;
			uint32_t got = s32xor(l, r);
			if (expected != got) {
				printf("S32XOR expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32XOR, pass);
	}

	// test S32OR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected = l | r;
			uint32_t got = s32or(l, r);
			if (expected != got) {
				printf("S32OR expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32OR, pass);
	}
}
