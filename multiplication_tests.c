#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

#define GEN_D16MUL_insn(optn2) \
static void d16mul##optn2(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"d16mul $xr1, $xr2, $xr1, $xr2, %[ptn]\n" \
		"s32std $xr1, %U[l32]\n" \
		"s32std $xr2, %U[r32]\n" \
		: [l32] "=*m"(*l32), [r32] "=*m"(*r32) \
		: [x] "r"(x), [y] "r"(y), [ptn] "i"(optn2) \
		: "xr1", "xr2" \
	); \
}

GEN_D16MUL_insn(0)
GEN_D16MUL_insn(1)
GEN_D16MUL_insn(2)
GEN_D16MUL_insn(3)

typedef void (*d16mul_proto_t)(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y);

static void d16mul(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y, int optn2)
{
	const d16mul_proto_t fns[] = {
		d16mul0, d16mul1, d16mul2, d16mul3,
	};
	fns[optn2](l32, r32, x, y);
}

#define ROUNDING_OFF     0
#define ROUNDING_ON      1
#define ROUNDING_BIASED  3

#define GEN_D16MULF_insn(optn2) \
static uint32_t d16mulf##optn2(uint32_t x, uint32_t y, int rounding)  \
{ \
	s32i2m(16, (rounding << 1) | 1); \
	uint32_t ret; \
	asm volatile ( \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"d16mulf $xr1,$xr1, $xr2, %[ptn]\n" \
		"s32m2i $xr1, %[ret]\n " \
		: [ret] "=r"(ret) \
		: [x] "r"(x), [y] "r"(y), [ptn] "i"(optn2) \
		: "xr1", "xr2" \
	); \
	return ret; \
}

GEN_D16MULF_insn(0)
GEN_D16MULF_insn(1)
GEN_D16MULF_insn(2)
GEN_D16MULF_insn(3)

typedef uint32_t (*d16mulf_proto_t)(uint32_t x, uint32_t y, int rounding);

static uint32_t d16mulf(uint32_t x, uint32_t y, int rounding, int optn2)
{
	const d16mulf_proto_t fns[] = {
		d16mulf0, d16mulf1, d16mulf2, d16mulf3,
	};
	if (rounding >= 2)
	    rounding = 3;
	return fns[optn2](x, y, rounding);
}

#define GEN_D16MULE_insn(optn2) \
static void d16mule##optn2(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y, int rounding) \
{ \
	s32i2m(16, (rounding << 1) | 1); \
	asm volatile ( \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"d16mule $xr1, $xr2, $xr1, $xr2, %[ptn]\n" \
		"s32std $xr1, %U[l32]\n" \
		"s32std $xr2, %U[r32]\n" \
		: [l32] "=*m"(*l32), [r32] "=*m"(*r32) \
		: [x] "r"(x), [y] "r"(y), [ptn] "i"(optn2) \
		: "xr1", "xr2" \
	); \
}

GEN_D16MULE_insn(0)
GEN_D16MULE_insn(1)
GEN_D16MULE_insn(2)
GEN_D16MULE_insn(3)

typedef void (*d16mule_proto_t)(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y, int rounding);

static void d16mule(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y, int rounding, int optn2)
{
	const d16mule_proto_t fns[] = {
		d16mule0, d16mule1, d16mule2, d16mule3,
	};
	if (rounding >= 2)
	    rounding = 3;
	fns[optn2](l32, r32, x, y, rounding);
}

#define GEN_D16MAC_insn(aptn2, optn2) \
static void d16mac##aptn2##optn2(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"s32ldd $xr3, %U[l32]\n" \
		"s32ldd $xr4, %U[r32]\n" \
		"d16mac $xr3, $xr4, $xr1, $xr2, %[aptn], %[optn]\n" \
		"s32std $xr3, %U[l32]\n" \
		"s32std $xr4, %U[r32]\n" \
		: [l32] "+*m"(*l32), [r32] "+*m"(*r32) \
		: [x] "r"(x), [y] "r"(y), [aptn] "i"(aptn2), [optn] "i"(optn2) \
		: "xr1", "xr2", "xr3", "xr4" \
	); \
}

GEN_D16MAC_insn(0,0)
GEN_D16MAC_insn(0,1)
GEN_D16MAC_insn(0,2)
GEN_D16MAC_insn(0,3)
GEN_D16MAC_insn(1,0)
GEN_D16MAC_insn(1,1)
GEN_D16MAC_insn(1,2)
GEN_D16MAC_insn(1,3)
GEN_D16MAC_insn(2,0)
GEN_D16MAC_insn(2,1)
GEN_D16MAC_insn(2,2)
GEN_D16MAC_insn(2,3)
GEN_D16MAC_insn(3,0)
GEN_D16MAC_insn(3,1)
GEN_D16MAC_insn(3,2)
GEN_D16MAC_insn(3,3)

typedef void (*d16mac_proto_t)(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y);

static void d16mac(uint32_t *l32, uint32_t *r32, uint32_t x, uint32_t y, int aptn2, int optn2)
{
	const d16mac_proto_t fns[4][4] = {
		{ d16mac00, d16mac01, d16mac02, d16mac03, },
		{ d16mac10, d16mac11, d16mac12, d16mac13, },
		{ d16mac20, d16mac21, d16mac22, d16mac23, },
		{ d16mac30, d16mac31, d16mac32, d16mac33, },
	};
	fns[aptn2][optn2](l32, r32, x, y);
}

#define GEN_D16MACF_insn(aptn2,optn2) \
static void d16macf##aptn2##optn2(uint32_t *acc, uint32_t *acc2, uint32_t x, uint32_t y, int rounding) \
{ \
	s32i2m(16, (rounding << 1) | 1); \
	asm volatile (  \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"s32ldd $xr3, %U[acc]\n" \
		"s32ldd $xr4, %U[acc2]\n" \
		"d16macf $xr3, $xr4, $xr1, $xr2, %[aptn], %[optn]\n" \
		"s32std $xr3, %U[acc]\n" \
		: [acc] "+*m"(*acc) \
		: [x] "r"(x), [y] "r"(y), [acc2] "*m"(*acc2), [aptn] "i"(aptn2), [optn] "i"(optn2) \
		: "xr1", "xr2", "xr3", "xr4" \
	); \
}

GEN_D16MACF_insn(0,0)
GEN_D16MACF_insn(0,1)
GEN_D16MACF_insn(0,2)
GEN_D16MACF_insn(0,3)
GEN_D16MACF_insn(1,0)
GEN_D16MACF_insn(1,1)
GEN_D16MACF_insn(1,2)
GEN_D16MACF_insn(1,3)
GEN_D16MACF_insn(2,0)
GEN_D16MACF_insn(2,1)
GEN_D16MACF_insn(2,2)
GEN_D16MACF_insn(2,3)
GEN_D16MACF_insn(3,0)
GEN_D16MACF_insn(3,1)
GEN_D16MACF_insn(3,2)
GEN_D16MACF_insn(3,3)

typedef void (*d16macx_proto_t)(uint32_t *acc, uint32_t *acc2, uint32_t x, uint32_t y, int rounding);

static void d16macf(uint32_t *acc, uint32_t *acc2, uint32_t x, uint32_t y, int rounding, int aptn2, int optn2)
{
	const d16macx_proto_t fns[4][4] = {
		{ d16macf00, d16macf01, d16macf02, d16macf03, },
		{ d16macf10, d16macf11, d16macf12, d16macf13, },
		{ d16macf20, d16macf21, d16macf22, d16macf23, },
		{ d16macf30, d16macf31, d16macf32, d16macf33, },
	};
	if (rounding >= 2)
	    rounding = 3;
	fns[aptn2][optn2](acc, acc2, x, y, rounding);
}

#define GEN_D16MACE_insn(aptn2,optn2) \
static void d16mace##aptn2##optn2(uint32_t *acc, uint32_t *acc2, uint32_t x, uint32_t y, int rounding) \
{ \
	s32i2m(16, (rounding << 1) | 1); \
	asm volatile (  \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"s32ldd $xr3, %U[acc]\n" \
		"s32ldd $xr4, %U[acc2]\n" \
		"d16mace $xr3, $xr4, $xr1, $xr2, %[aptn], %[optn]\n" \
		"s32std $xr3, %U[acc]\n" \
		"s32std $xr4, %U[acc2]\n" \
		: [acc] "+*m"(*acc), [acc2] "+*m"(*acc2) \
		: [x] "r"(x), [y] "r"(y), [aptn] "i"(aptn2), [optn] "i"(optn2) \
		: "xr1", "xr2", "xr3", "xr4" \
	); \
}

GEN_D16MACE_insn(0,0)
GEN_D16MACE_insn(0,1)
GEN_D16MACE_insn(0,2)
GEN_D16MACE_insn(0,3)
GEN_D16MACE_insn(1,0)
GEN_D16MACE_insn(1,1)
GEN_D16MACE_insn(1,2)
GEN_D16MACE_insn(1,3)
GEN_D16MACE_insn(2,0)
GEN_D16MACE_insn(2,1)
GEN_D16MACE_insn(2,2)
GEN_D16MACE_insn(2,3)
GEN_D16MACE_insn(3,0)
GEN_D16MACE_insn(3,1)
GEN_D16MACE_insn(3,2)
GEN_D16MACE_insn(3,3)

static void d16mace(uint32_t *acc, uint32_t *acc2, uint32_t x, uint32_t y, int rounding, int aptn2, int optn2)
{
	const d16macx_proto_t fns[4][4] = {
		{ d16mace00, d16mace01, d16mace02, d16mace03, },
		{ d16mace10, d16mace11, d16mace12, d16mace13, },
		{ d16mace20, d16mace21, d16mace22, d16mace23, },
		{ d16mace30, d16mace31, d16mace32, d16mace33, },
	};
	if (rounding >= 2)
	    rounding = 3;
	fns[aptn2][optn2](acc, acc2, x, y, rounding);
}

#define GEN_S16MAD_insn(aptn1, optn2) \
static uint32_t s16mad##aptn1##optn2(uint32_t acc, uint32_t x, uint32_t y)  \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"s32i2m $xr3, %[acc]\n" \
		"s16mad $xr1, $xr3, $xr1, $xr2, %[aptn], %[optn]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret) \
		: [x] "r"(x), [y] "r"(y), [acc] "r"(acc), [aptn] "i"(aptn1), [optn] "i"(optn2) \
		: "xr1", "xr2", "xr3" \
	); \
	return ret; \
}

GEN_S16MAD_insn(0,0)
GEN_S16MAD_insn(0,1)
GEN_S16MAD_insn(0,2)
GEN_S16MAD_insn(0,3)
GEN_S16MAD_insn(1,0)
GEN_S16MAD_insn(1,1)
GEN_S16MAD_insn(1,2)
GEN_S16MAD_insn(1,3)

typedef uint32_t (*s16mad_proto_t)(uint32_t acc, uint32_t x, uint32_t y);

static uint32_t s16mad(uint32_t acc, uint32_t x, uint32_t y, int aptn1, int optn2)
{
	const s16mad_proto_t fns[2][4] = {
		{ s16mad00, s16mad01, s16mad02, s16mad03, },
		{ s16mad10, s16mad11, s16mad12, s16mad13, },
	};
	return fns[aptn1][optn2](acc, x, y);
}

#define GEN_D16MADL_insn(aptn2, optn2) \
static uint32_t d16madl##aptn2##optn2(uint32_t acc, uint32_t x, uint32_t y) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"s32i2m $xr3, %[acc]\n" \
		"d16madl $xr1, $xr3, $xr1, $xr2, %[aptn], %[optn]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret) \
		: [x] "r"(x), [y] "r"(y), [acc] "r"(acc), [aptn] "i"(aptn2), [optn] "i"(optn2) \
		: "xr1", "xr2", "xr3" \
	); \
	return ret; \
}

GEN_D16MADL_insn(0,0)
GEN_D16MADL_insn(0,1)
GEN_D16MADL_insn(0,2)
GEN_D16MADL_insn(0,3)
GEN_D16MADL_insn(1,0)
GEN_D16MADL_insn(1,1)
GEN_D16MADL_insn(1,2)
GEN_D16MADL_insn(1,3)
GEN_D16MADL_insn(2,0)
GEN_D16MADL_insn(2,1)
GEN_D16MADL_insn(2,2)
GEN_D16MADL_insn(2,3)
GEN_D16MADL_insn(3,0)
GEN_D16MADL_insn(3,1)
GEN_D16MADL_insn(3,2)
GEN_D16MADL_insn(3,3)

static uint32_t d16madl(uint32_t acc, uint32_t x, uint32_t y, int aptn2, int optn2)
{
	const s16mad_proto_t fns[4][4] = {
		{ d16madl00, d16madl01, d16madl02, d16madl03, },
		{ d16madl10, d16madl11, d16madl12, d16madl13, },
		{ d16madl20, d16madl21, d16madl22, d16madl23, },
		{ d16madl30, d16madl31, d16madl32, d16madl33, },
	};
	return fns[aptn2][optn2](acc, x, y);
}


static int64_t s32mul(uint32_t *lo_hi, int32_t x, int32_t y)
{
	uint32_t ret_hi, ret_lo;
	asm volatile (
		"s32mul $xr1, $xr2, %[x], %[y]\n"
		"s32m2i $xr1, %[ret_hi]\n"
		"s32m2i $xr2, %[ret_lo]\n"
		"mfhi %[x]\n"
		"sw %[x], %[mhi]\n"
		"mflo %[x]\n"
		"sw %[x], %[mlo]\n"
		: [ret_hi] "=r"(ret_hi), [ret_lo] "=r"(ret_lo), [mlo] "=m"(lo_hi[0]), [mhi] "=m"(lo_hi[1])
		: [x] "r"(x), [y] "r"(y)
		: "xr1", "xr2", "hi", "lo"
	);
	return (int64_t)(((uint64_t)ret_hi << 32) | ret_lo);
}

static uint64_t s32mulu(uint32_t *lo_hi, uint32_t x, uint32_t y)
{
	uint32_t ret_hi, ret_lo;
	asm volatile (
		"s32mulu $xr1, $xr2, %[x], %[y]\n"
		"s32m2i $xr1, %[ret_hi]\n"
		"s32m2i $xr2, %[ret_lo]\n"
		"mfhi %[x]\n"
		"sw %[x], %[mhi]\n"
		"mflo %[x]\n"
		"sw %[x], %[mlo]\n"
		: [ret_hi] "=r"(ret_hi), [ret_lo] "=r"(ret_lo),
		  [mlo] "=m"(lo_hi[0]), [mhi] "=m"(lo_hi[1]), [x] "+r"(x)
		: [y] "r"(y)
		: "xr1", "xr2", "hi", "lo"
	);
	return (((uint64_t)ret_hi << 32) | ret_lo);
}

static void s32madd(int64_t *addr, uint32_t *hilo, int32_t x, int32_t y)
{
	asm volatile (
		"s32ldd $xr1, %U[addr]\n"
		"s32ldd $xr2, %U[addr2]\n"
		"s32madd $xr2, $xr1, %[x], %[y]\n"
		"s32std $xr1, %U[addr]\n"
		"s32std $xr2, %U[addr2]\n"
		"mfhi %[x]\n"
		"sw %[x], %[mhi]\n"
		"mflo %[x]\n"
		"sw %[x], %[mlo]\n"
		: [addr] "+m"(((uint32_t*)addr)[0]), [addr2] "+m"(((uint32_t*)addr)[1]),
		  [mhi] "=m"(hilo[1]), [mlo] "=m"(hilo[0]), [x] "+r"(x)
		: [y] "r"(y)
		: "xr1", "xr2", "hi", "lo"
	);
}

static void s32maddu(uint64_t *addr, uint32_t *hilo, uint32_t x, uint32_t y)
{
	asm volatile (
		"s32ldd $xr1, %U[addr]\n"
		"s32ldd $xr2, %U[addr2]\n"
		"s32maddu $xr2, $xr1, %[x], %[y]\n"
		"s32std $xr1, %U[addr]\n"
		"s32std $xr2, %U[addr2]\n"
		"mfhi %[x]\n"
		"sw %[x], %[mhi]\n"
		"mflo %[x]\n"
		"sw %[x], %[mlo]\n"
		: [addr] "+m"(((uint32_t*)addr)[0]), [addr2] "+m"(((uint32_t*)addr)[1]),
		  [mhi] "=m"(hilo[1]), [mlo] "=m"(hilo[0]), [x] "+r"(x)
		: [y] "r"(y)
		: "xr1", "xr2", "hi", "lo"
	);
}

static void s32msub(int64_t *addr, uint32_t *hilo, int32_t x, int32_t y)
{
	asm volatile (
		"s32ldd $xr1, %U[addr]\n"
		"s32ldd $xr2, %U[addr2]\n"
		"s32msub $xr2, $xr1, %[x], %[y]\n"
		"s32std $xr1, %U[addr]\n"
		"s32std $xr2, %U[addr2]\n"
		"mfhi %[x]\n"
		"sw %[x], %[mhi]\n"
		"mflo %[x]\n"
		"sw %[x], %[mlo]\n"
		: [addr] "+m"(((uint32_t*)addr)[0]), [addr2] "+m"(((uint32_t*)addr)[1]),
		  [mhi] "=m"(hilo[1]), [mlo] "=m"(hilo[0]), [x] "+r"(x)
		: [y] "r"(y)
		: "xr1", "xr2", "hi", "lo"
	);
}

static void s32msubu(uint64_t *addr, uint32_t *hilo, uint32_t x, uint32_t y)
{
	asm volatile (
		"s32ldd $xr1, %U[addr]\n"
		"s32ldd $xr2, %U[addr2]\n"
		"s32msubu $xr2, $xr1, %[x], %[y]\n"
		"s32std $xr1, %U[addr]\n"
		"s32std $xr2, %U[addr2]\n"
		"mfhi %[x]\n"
		"sw %[x], %[mhi]\n"
		"mflo %[x]\n"
		"sw %[x], %[mlo]\n"
		: [addr] "+m"(((uint32_t*)addr)[0]), [addr2] "+m"(((uint32_t*)addr)[1]),
		  [mhi] "=m"(hilo[1]), [mlo] "=m"(hilo[0]), [x] "+r"(x)
		: [y] "r"(y)
		: "xr1", "xr2", "hi", "lo"
	);
}

static void q8mul(uint32_t *ret, uint32_t x, uint32_t y)
{
	asm volatile (
		"s32i2m $xr1, %[x]\n"
		"s32i2m $xr2, %[y]\n"
		"q8mul $xr1, $xr2, $xr1, $xr2\n"
		"s32std $xr1, %U[ret_a]\n"
		"s32std $xr2, %U[ret_d]\n"
		: [ret_a] "=m"(ret[1]), [ret_d] "=m"(ret[0])
		: [x] "r"(x), [y] "r"(y)
		: "xr1", "xr2"
	);
}

static void q8mulsu(uint32_t *ret, uint32_t x, uint32_t y)
{
	asm volatile (
		"s32i2m $xr1, %[x]\n"
		"s32i2m $xr2, %[y]\n"
		"q8mulsu $xr1, $xr2, $xr1, $xr2\n"
		"s32std $xr1, %U[ret_a]\n"
		"s32std $xr2, %U[ret_d]\n"
		: [ret_a] "=m"(ret[1]), [ret_d] "=m"(ret[0])
		: [x] "r"(x), [y] "r"(y)
		: "xr1", "xr2"
	);
}

#define GEN_Q8MAC_insn(aptn2) \
static void q8mac##aptn2(uint32_t *ret, uint32_t x, uint32_t y) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"s32ldd $xr3, %U[ret_a]\n" \
		"s32ldd $xr4, %U[ret_d]\n" \
		"q8mac $xr3, $xr4, $xr1, $xr2, %[ptn]\n" \
		"s32std $xr3, %U[ret_a]\n" \
		"s32std $xr4, %U[ret_d]\n" \
		: [ret_a] "+m"(ret[1]), [ret_d] "+m"(ret[0]) \
		: [x] "r"(x), [y] "r"(y), [ptn] "i"(aptn2) \
		: "xr1", "xr2", "xr3", "xr4" \
	); \
}

GEN_Q8MAC_insn(0)
GEN_Q8MAC_insn(1)
GEN_Q8MAC_insn(2)
GEN_Q8MAC_insn(3)

typedef void (*q8macxx_proto_t)(uint32_t *acc, uint32_t x, uint32_t y);

static void q8mac(uint32_t *acc, uint32_t x, uint32_t y, int aptn2)
{
	const q8macxx_proto_t fns[4] = {
		q8mac0, q8mac1, q8mac2, q8mac3,
	};
	fns[aptn2](acc, x, y);
}

#define GEN_Q8MACSU_insn(aptn2) \
static void q8macsu##aptn2(uint32_t *ret, uint32_t x, uint32_t y) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[x]\n" \
		"s32i2m $xr2, %[y]\n" \
		"s32ldd $xr3, %U[ret_a]\n" \
		"s32ldd $xr4, %U[ret_d]\n" \
		"q8macsu $xr3, $xr4, $xr1, $xr2, %[ptn]\n" \
		"s32std $xr3, %U[ret_a]\n" \
		"s32std $xr4, %U[ret_d]\n" \
		: [ret_a] "+m"(ret[1]), [ret_d] "+m"(ret[0]) \
		: [x] "r"(x), [y] "r"(y), [ptn] "i"(aptn2) \
		: "xr1", "xr2", "xr3", "xr4" \
	); \
}

GEN_Q8MACSU_insn(0)
GEN_Q8MACSU_insn(1)
GEN_Q8MACSU_insn(2)
GEN_Q8MACSU_insn(3)

static void q8macsu(uint32_t *acc, uint32_t x, uint32_t y, int aptn2)
{
	const q8macxx_proto_t fns[4] = {
		q8macsu0, q8macsu1, q8macsu2, q8macsu3,
	};
	fns[aptn2](acc, x, y);
}

#define GEN_Q8MADL_insn(aptn2) \
static uint32_t q8madl##aptn2(uint32_t a, uint32_t b, uint32_t c) \
{ \
	uint32_t d; \
	asm volatile ( \
		"s32i2m $xr1, %[b]\n" \
		"s32i2m $xr2, %[c]\n" \
		"s32i2m $xr3, %[a]\n" \
		"q8madl $xr3, $xr3, $xr1, $xr2, %[ptn]\n" \
		"s32m2i $xr3, %[d]\n" \
		: [d] "=r"(d) \
		: [a] "r"(a), [b] "r"(b), [c] "r"(c), [ptn] "i"(aptn2) \
		: "xr1", "xr2", "xr3" \
	); \
	return d; \
}

GEN_Q8MADL_insn(0)
GEN_Q8MADL_insn(1)
GEN_Q8MADL_insn(2)
GEN_Q8MADL_insn(3)

typedef uint32_t (*q8madl_proto_t)(uint32_t a, uint32_t b, uint32_t c);

static uint32_t q8madl(uint32_t a, uint32_t b, uint32_t c, int aptn2)
{
	const q8madl_proto_t fns[4] = {
		q8madl0, q8madl1, q8madl2, q8madl3,
	};
	return fns[aptn2](a, b, c);
}

void test_multiplications(int num_passes)
{
	// test S32MUL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t x = random32();
			uint32_t y = random32();
			int64_t expected = (int64_t)(int32_t)x * (int64_t)(int32_t)y;
			int64_t got, hilo;
			uint32_t hi_lo[2] = { 0 };
			got = s32mul(hi_lo, x, y);
			hilo = (((uint64_t)hi_lo[1]) << 32) | hi_lo[0];
			if (got != expected || hilo != expected) {
				/* FIXME if you remove x and y from printf args test is going to fail */
				printf("S32MUL expected: %lld, got %lld %lld %u %u\n",
					expected, got, hilo, x, y);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MUL, pass);
	}

	// test S32MULU
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t x = random32();
			uint32_t y = random32();
			uint64_t expected = (uint64_t)x * y;
			uint64_t got, hilo;
			uint32_t hi_lo[2];
			got = s32mulu(hi_lo, x, y);
			hilo = ((uint64_t)hi_lo[1] << 32) | hi_lo[0];
			if (got != expected || hilo != expected) {
				printf("S32MULU expected: %llu, got %llu %llu\n",
					expected, got, hilo);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MULU, pass);
	}

	// test S32MADD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t x = random32();
			uint32_t y = random32();
			int64_t init = random64();
			int64_t expected = init + (int64_t)(int32_t)x * (int64_t)(int32_t)y;
			int64_t got = init, hilo;
			uint32_t *hi_lo = (uint32_t *)&hilo;
			s32madd(&got, hi_lo, x, y);
			if (got != expected || hilo != expected) {
				printf("S32MADD expected: %lld, got %lld, %lld\n",
					expected, got, hilo);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MADD, pass);
	}

	// test S32MADDU
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t x = random32();
			uint32_t y = random32();
			uint64_t init = random64();
			uint64_t expected = init + (uint64_t)x * y;
			uint64_t got = init, hilo;
			uint32_t *hi_lo = (uint32_t *)&hilo;
			s32maddu(&got, hi_lo, x, y);
			if (got != expected || hilo != expected) {
				printf("S32MADDU expected: %llu, got %llu %llu\n",
					expected, got, hilo);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MADDU, pass);
	}

	// test S32MSUB
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t x = random32();
			uint32_t y = random32();
			int64_t init = random64();
			int64_t expected = init - (int64_t)(int32_t)x * (int64_t)(int32_t)y;
			int64_t got = init, hilo;
			uint32_t *hi_lo = (uint32_t *)&hilo;
			s32msub(&got, hi_lo, x, y);
			if (got != expected || hilo != expected) {
				printf("S32MSUB expected: %lld, got %lld %lld\n",
					expected, got, hilo);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MSUB, pass);
	}

	// test S32MSUBU
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t x = random32();
			uint32_t y = random32();
			uint64_t init = random64();
			uint64_t expected = init - (uint64_t)x * y;
			uint64_t got = init, hilo;
			uint32_t *hi_lo = (uint32_t *)&hilo;
			s32msubu(&got, hi_lo, x, y);
			if (got != expected || hilo != expected) {
				printf("S32MSUBU expected: %llu, got %llu %llu\n",
					expected, got, hilo);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32MSUBU, pass);
	}

	// test D16MUL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int optn2 = random() % 4;
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected_l = 0, expected_r = 0, got_l, got_r;
			int16_t l_hi = l >> 16, l_lo = l, r_hi = r >> 16, r_lo = r;
			switch (optn2) {
			case 0:
				expected_l = (int32_t)l_hi * r_hi;
				expected_r = (int32_t)l_lo * r_lo;
				break;
			case 1:
				expected_l = (int32_t)l_lo * r_hi;
				expected_r = (int32_t)l_lo * r_lo;
				break;
			case 2:
				expected_l = (int32_t)l_hi * r_hi;
				expected_r = (int32_t)l_hi * r_lo;
				break;
			case 3:
				expected_l = (int32_t)l_lo * r_hi;
				expected_r = (int32_t)l_hi * r_lo;
				break;
			}
			d16mul(&got_l, &got_r, l, r, optn2);
			if (expected_l != got_l || expected_r != got_r) {
				printf("D16MUL expected: %08x %08x got: %08x %08x\n",
				       expected_l, expected_r, got_l, got_r);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MUL, pass);
	}

	// test D16MULF
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int optn2 = random() % 4;
			int rounding = random() % 3;
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected;
			int16_t l_hi = l >> 16, l_lo = l, r_hi = r >> 16, r_lo = r;
			int32_t hi = 0, lo = 0;
			switch (optn2) {
			case 0:
				hi = ((int32_t)l_hi * r_hi) << 1;
				lo = ((int32_t)l_lo * r_lo) << 1;
				break;
			case 1:
				hi = ((int32_t)l_lo * r_hi) << 1;
				lo = ((int32_t)l_lo * r_lo) << 1;
				break;
			case 2:
				hi = ((int32_t)l_hi * r_hi) << 1;
				lo = ((int32_t)l_hi * r_lo) << 1;
				break;
			case 3:
				hi = ((int32_t)l_lo * r_hi) << 1;
				lo = ((int32_t)l_hi * r_lo) << 1;
				break;
			}
			switch (rounding) {
			case 0:
				break;
			case 1:
				if ((hi & 0x1ffff) != 0x08000)
					hi += 0x8000;
				if ((lo & 0x1ffff) != 0x08000)
					lo += 0x8000;
				break;
			case 2:
				hi += 0x8000;
				lo += 0x8000;
				break;
			}
			expected  = hi & 0xffff0000;
			expected |= (lo >> 16) & 0x0000ffff;
			uint32_t got = d16mulf(l, r, rounding, optn2);
			if (expected != got) {
				printf("D16MULF expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MULF, pass);
	}

	// test D16MULE
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int optn2 = random() % 4;
			int rounding = random() % 3;
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected_l, expected_r, got_l, got_r;
			int16_t l_hi = l >> 16, l_lo = l, r_hi = r >> 16, r_lo = r;
			int32_t hi = 0, lo = 0;
			switch (optn2) {
			case 0:
				hi = ((int32_t)l_hi * r_hi) << 1;
				lo = ((int32_t)l_lo * r_lo) << 1;
				break;
			case 1:
				hi = ((int32_t)l_lo * r_hi) << 1;
				lo = ((int32_t)l_lo * r_lo) << 1;
				break;
			case 2:
				hi = ((int32_t)l_hi * r_hi) << 1;
				lo = ((int32_t)l_hi * r_lo) << 1;
				break;
			case 3:
				hi = ((int32_t)l_lo * r_hi) << 1;
				lo = ((int32_t)l_hi * r_lo) << 1;
				break;
			}
			switch (rounding) {
			case 0:
				break;
			case 1:
			case 2:
				hi += 0x8000;
				lo += 0x8000;
				break;
			}
			expected_l = hi;
			expected_r = lo;
			d16mule(&got_l, &got_r, l, r, rounding, optn2);
			if (expected_l != got_l || expected_r != got_r) {
				printf("D16MULE expected: %08x %08x got: %08x %08x\n",
				       expected_l, expected_r, got_l, got_r);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MULE, pass);
	}

	// test D16MAC
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int optn2 = random() % 4;
			int aptn2 = random() % 4;
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected_l, expected_r, mul_l = 0, mul_r = 0, got_l, got_r;
			int16_t l_hi = l >> 16, l_lo = l, r_hi = r >> 16, r_lo = r;
			switch (optn2) {
			case 0:
				mul_l = (int32_t)l_hi * r_hi;
				mul_r = (int32_t)l_lo * r_lo;
				break;
			case 1:
				mul_l = (int32_t)l_lo * r_hi;
				mul_r = (int32_t)l_lo * r_lo;
				break;
			case 2:
				mul_l = (int32_t)l_hi * r_hi;
				mul_r = (int32_t)l_hi * r_lo;
				break;
			case 3:
				mul_l = (int32_t)l_lo * r_hi;
				mul_r = (int32_t)l_hi * r_lo;
				break;
			}
			got_l = expected_l = random32();
			got_r = expected_r = random32();
			switch (aptn2) {
			case 0:
				expected_l += mul_l;
				expected_r += mul_r;
				break;
			case 1:
				expected_l += mul_l;
				expected_r -= mul_r;
				break;
			case 2:
				expected_l -= mul_l;
				expected_r += mul_r;
				break;
			case 3:
				expected_l -= mul_l;
				expected_r -= mul_r;
				break;
			}
			d16mac(&got_l, &got_r, l, r, aptn2, optn2);
			if (expected_l != got_l || expected_r != got_r) {
				printf("D16MAC expected: %08x %08x got: %08x %08x\n",
				       expected_l, expected_r, got_l, got_r);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MAC, pass);
	}

	// test D16MACF
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int aptn2 = random() % 4;
			int optn2 = random() % 4;
			int rounding = random() % 3;
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected, sum_l, sum_r, mul_l = 0, mul_r = 0, got;
			int16_t l_hi = l >> 16, l_lo = l, r_hi = r >> 16, r_lo = r;
			got = sum_l = random32();
			sum_r = random32();
			switch (optn2) {
			case 0:
				mul_l = (int32_t)l_hi * r_hi;
				mul_r = (int32_t)l_lo * r_lo;
				break;
			case 1:
				mul_l = (int32_t)l_lo * r_hi;
				mul_r = (int32_t)l_lo * r_lo;
				break;
			case 2:
				mul_l = (int32_t)l_hi * r_hi;
				mul_r = (int32_t)l_hi * r_lo;
				break;
			case 3:
				mul_l = (int32_t)l_lo * r_hi;
				mul_r = (int32_t)l_hi * r_lo;
				break;
			}
			mul_l <<= 1;
			mul_r <<= 1;
			switch (aptn2) {
			case 0:
				mul_l = sum_l + mul_l;
				mul_r = sum_r + mul_r;
				break;
			case 1:
				mul_l = sum_l + mul_l;
				mul_r = sum_r - mul_r;
				break;
			case 2:
				mul_l = sum_l - mul_l;
				mul_r = sum_r + mul_r;
				break;
			case 3:
				mul_l = sum_l - mul_l;
				mul_r = sum_r - mul_r;
				break;
			}
			switch (rounding) {
			case 0:
				break;
			case 1:
				if ((mul_l & 0x1ffff) != 0x08000)
					mul_l += 0x8000;
				if ((mul_r & 0x1ffff) != 0x08000)
					mul_r += 0x8000;
				break;
			case 2:
				mul_l += 0x8000;
				mul_r += 0x8000;
				break;
			}
			expected  = mul_l & 0xffff0000;
			expected |= (mul_r >> 16) & 0x0000ffff;
			d16macf(&got, &sum_r, l, r, rounding, aptn2, optn2);
			if (expected != got) {
				printf("D16MACF expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MACF, pass);
	}

	// test D16MACE
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int optn2 = random() % 4;
			int aptn2 = random() % 4;
			int rounding = random() % 3;
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected_l, expected_r, sum_l, sum_r, mul_l = 0, mul_r = 0, got_l, got_r;
			int16_t l_hi = l >> 16, l_lo = l, r_hi = r >> 16, r_lo = r;
			got_l = sum_l = random32();
			got_r = sum_r = random32();
			switch (optn2) {
			case 0:
				mul_l = (int32_t)l_hi * r_hi;
				mul_r = (int32_t)l_lo * r_lo;
				break;
			case 1:
				mul_l = (int32_t)l_lo * r_hi;
				mul_r = (int32_t)l_lo * r_lo;
				break;
			case 2:
				mul_l = (int32_t)l_hi * r_hi;
				mul_r = (int32_t)l_hi * r_lo;
				break;
			case 3:
				mul_l = (int32_t)l_lo * r_hi;
				mul_r = (int32_t)l_hi * r_lo;
				break;
			}
			mul_l <<= 1;
			mul_r <<= 1;
			switch (aptn2) {
			case 0:
				mul_l = sum_l + mul_l;
				mul_r = sum_r + mul_r;
				break;
			case 1:
				mul_l = sum_l + mul_l;
				mul_r = sum_r - mul_r;
				break;
			case 2:
				mul_l = sum_l - mul_l;
				mul_r = sum_r + mul_r;
				break;
			case 3:
				mul_l = sum_l - mul_l;
				mul_r = sum_r - mul_r;
				break;
			}
			switch (rounding) {
			case 0:
				break;
			case 1:
			case 2:
				mul_l += 0x8000;
				mul_r += 0x8000;
				break;
			}
			expected_l = mul_l;
			expected_r = mul_r;
			d16mace(&got_l, &got_r, l, r, rounding, aptn2, optn2);
			if (expected_l != got_l || expected_r != got_r) {
				printf("D16MACE expected: %08x %08x got: %08x %08x %08x %08x %08x %08x %d %d %d\n",
				       expected_l, expected_r, got_l, got_r, l, r, sum_l, sum_r, optn2, aptn2, rounding);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MACE, pass);
	}

	// test S16MAD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int optn2 = random() % 4;
			int aptn1 = random() % 2;
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected = 0, acc, mul = 0, got;
			int16_t l_hi = l >> 16, l_lo = l, r_hi = r >> 16, r_lo = r;
			acc = random32();
			switch (optn2) {
			case 0:
				mul = (int32_t)l_hi * r_hi;
				break;
			case 1:
				mul = (int32_t)l_lo * r_lo;
				break;
			case 2:
				mul = (int32_t)l_hi * r_lo;
				break;
			case 3:
				mul = (int32_t)l_lo * r_hi;
				break;
			}
			switch (aptn1) {
			case 0:
				expected = acc + mul;
				break;
			case 1:
				expected = acc - mul;
				break;
			}
			got = s16mad(acc, l, r, aptn1, optn2);
			if (expected != got) {
				printf("S16MAD expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S16MAD, pass);
	}

	// test D16MADL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			int optn2 = random() % 4;
			int aptn2 = random() % 4;
			uint32_t l = random32();
			uint32_t r = random32();
			uint32_t expected, acc, mul_l = 0, mul_r = 0, got;
			int16_t l_hi = l >> 16, l_lo = l, r_hi = r >> 16, r_lo = r;
			acc = random32();
			switch (optn2) {
			case 0:
				mul_l = (int32_t)l_hi * r_hi;
				mul_r = (int32_t)l_lo * r_lo;
				break;
			case 1:
				mul_l = (int32_t)l_lo * r_hi;
				mul_r = (int32_t)l_lo * r_lo;
				break;
			case 2:
				mul_l = (int32_t)l_hi * r_hi;
				mul_r = (int32_t)l_hi * r_lo;
				break;
			case 3:
				mul_l = (int32_t)l_lo * r_hi;
				mul_r = (int32_t)l_hi * r_lo;
				break;
			}
			mul_l &= 0xffff;
			mul_r &= 0xffff;
			switch (aptn2) {
			case 0:
				mul_l = (acc >> 16) + mul_l;
				mul_r = (acc & 0xffff) + mul_r;
				break;
			case 1:
				mul_l = (acc >> 16) + mul_l;
				mul_r = (acc & 0xffff) - mul_r;
				break;
			case 2:
				mul_l = (acc >> 16) - mul_l;
				mul_r = (acc & 0xffff) + mul_r;
				break;
			case 3:
				mul_l = (acc >> 16) - mul_l;
				mul_r = (acc & 0xffff) - mul_r;
				break;
			}
			mul_l &= 0xffff;
			mul_r &= 0xffff;
			expected  = mul_l << 16;
			expected |= mul_r;
			got = d16madl(acc, l, r, aptn2, optn2);
			if (expected != got) {
				printf("D16MADL expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(D16MADL, pass);
	}

	// test Q8MUL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t x = random32();
			uint32_t y = random32();
			uint32_t expected[2] = { 0 };
			uint32_t got[2];
			expected[0] |= (x & 0xff) * (y & 0xff);
			expected[0] |= (((x >> 8) & 0xff) * ((y >> 8) & 0xff)) << 16;
			expected[1] |= (((x >> 16) & 0xff) * ((y >> 16) & 0xff));
			expected[1] |= (((x >> 24) & 0xff) * ((y >> 24) & 0xff)) << 16;
			q8mul(got, x, y);
			if (got[0] != expected[0] || got[1] != expected[1]) {
				printf("Q8MUL expected: %08x %08x, got %08x %08x\n",
					expected[0], expected[1], got[0], got[1]);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MUL, pass);
	}

	// test Q8MULSU
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			uint32_t x = random32();
			uint32_t y = random32();
			uint32_t expected[2] = { 0 };
			uint32_t got[2];
			expected[0] |= ((int8_t)(x & 0xff) * (y & 0xff)) & 0xffff;
			expected[0] |= ((int8_t)((x >> 8) & 0xff) * ((y >> 8) & 0xff)) << 16;
			expected[1] |= ((int8_t)((x >> 16) & 0xff) * ((y >> 16) & 0xff)) & 0xffff;
			expected[1] |= ((int8_t)((x >> 24) & 0xff) * ((y >> 24) & 0xff)) << 16;
			q8mulsu(got, x, y);
			if (got[0] != expected[0] || got[1] != expected[1]) {
				printf("Q8MULSU expected: %08x %08x, got %08x %08x\n",
					expected[0], expected[1], got[0], got[1]);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MULSU, pass);
	}

	// test Q8MAC
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			int aptn2 = random() % 4;
			uint32_t x = random32();
			uint32_t y = random32();
			uint32_t expected[2] = { 0 };
			uint32_t got[2];
			got[0] = random32();
			got[1] = random32();
			if (aptn2 & 1) {
				expected[0] |= (got[0] - (x & 0xff) * (y & 0xff)) & 0xffff;
				expected[0] |= (got[0] - ((((x >> 8) & 0xff) * ((y >> 8) & 0xff)) << 16)) & 0xffff0000;
			} else {
				expected[0] |= (got[0] + (x & 0xff) * (y & 0xff)) & 0xffff;
				expected[0] |= (got[0] + ((((x >> 8) & 0xff) * ((y >> 8) & 0xff)) << 16)) & 0xffff0000;
			}
			if (aptn2 & 2) {
				expected[1] |= (got[1] - (((x >> 16) & 0xff) * ((y >> 16) & 0xff))) & 0xffff;
				expected[1] |= (got[1] - ((((x >> 24) & 0xff) * ((y >> 24) & 0xff)) << 16)) & 0xffff0000;
			} else {
				expected[1] |= (got[1] + (((x >> 16) & 0xff) * ((y >> 16) & 0xff))) & 0xffff;
				expected[1] |= (got[1] + ((((x >> 24) & 0xff) * ((y >> 24) & 0xff)) << 16)) & 0xffff0000;
			}
			q8mac(got, x, y, aptn2);
			if (got[0] != expected[0] || got[1] != expected[1]) {
				printf("Q8MAC expected: %08x %08x, got %08x %08x\n",
					expected[0], expected[1], got[0], got[1]);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MAC, pass);
	}

	// test Q8MACSU
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			int aptn2 = random() % 4;
			uint32_t x = random32();
			uint32_t y = random32();
			uint32_t expected[2] = { 0 };
			uint32_t got[2];
			got[0] = random32();
			got[1] = random32();
			if (aptn2 & 1) {
				expected[0] |= (got[0] - (int8_t)(x & 0xff) * (y & 0xff)) & 0xffff;
				expected[0] |= (got[0] - (((int8_t)((x >> 8) & 0xff) * ((y >> 8) & 0xff)) << 16)) & 0xffff0000;
			} else {
				expected[0] |= (got[0] + (int8_t)(x & 0xff) * (y & 0xff)) & 0xffff;
				expected[0] |= (got[0] + (((int8_t)((x >> 8) & 0xff) * ((y >> 8) & 0xff)) << 16)) & 0xffff0000;
			}
			if (aptn2 & 2) {
				expected[1] |= (got[1] - ((int8_t)((x >> 16) & 0xff) * ((y >> 16) & 0xff))) & 0xffff;
				expected[1] |= (got[1] - (((int8_t)((x >> 24) & 0xff) * ((y >> 24) & 0xff)) << 16)) & 0xffff0000;
			} else {
				expected[1] |= (got[1] + ((int8_t)((x >> 16) & 0xff) * ((y >> 16) & 0xff))) & 0xffff;
				expected[1] |= (got[1] + (((int8_t)((x >> 24) & 0xff) * ((y >> 24) & 0xff)) << 16)) & 0xffff0000;
			}
			q8macsu(got, x, y, aptn2);
			if (got[0] != expected[0] || got[1] != expected[1]) {
				printf("Q8MACSU expected: %08x %08x, got %08x %08x\n",
					expected[0], expected[1], got[0], got[1]);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MACSU, pass);
	}

	// test Q8MADL
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++) {
			int aptn2 = random() % 4;
			uint32_t b = random32();
			uint32_t c = random32();
			uint32_t a = random32();
			uint32_t expected = 0, d = 0;
			uint8_t *pa = (uint8_t *)&a, *pb = (uint8_t *)&b;
			uint8_t *pc = (uint8_t *)&c, *pd = (uint8_t *)&expected;
			if (aptn2 & 1) {
				pd[0] = pa[0] - pb[0] * pc[0];
				pd[1] = pa[1] - pb[1] * pc[1];
			} else {
				pd[0] = pa[0] + pb[0] * pc[0];
				pd[1] = pa[1] + pb[1] * pc[1];
			}
			if (aptn2 & 2) {
				pd[2] = pa[2] - pb[2] * pc[2];
				pd[3] = pa[3] - pb[3] * pc[3];
			} else {
				pd[2] = pa[2] + pb[2] * pc[2];
				pd[3] = pa[3] + pb[3] * pc[3];
			}
			d = q8madl(a, b, c, aptn2);
			if (d != expected) {
				printf("Q8MADL expected: %08x, got %08x\n", expected, d);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(Q8MADL, pass);
	}
}
