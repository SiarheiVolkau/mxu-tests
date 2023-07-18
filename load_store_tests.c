#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

#define GEN_LX_insn(insn, strd2) \
static uint32_t lx##insn##strd2(const uint32_t *address, uint32_t offset) \
{ \
	uint32_t ret; \
	asm volatile ("lx" #insn " %[dst], %[base], %[off], %[shift]\n" \
	: [dst] "=r"(ret) \
	: [base] "r"(address), [off] "r"(offset), [shift] "i"(strd2) \
	: ); \
	return ret; \
}

GEN_LX_insn(w, 0)
GEN_LX_insn(w, 1)
GEN_LX_insn(w, 2)

GEN_LX_insn(h, 0)
GEN_LX_insn(h, 1)
GEN_LX_insn(h, 2)

GEN_LX_insn(hu, 0)
GEN_LX_insn(hu, 1)
GEN_LX_insn(hu, 2)

GEN_LX_insn(b, 0)
GEN_LX_insn(b, 1)
GEN_LX_insn(b, 2)

GEN_LX_insn(bu, 0)
GEN_LX_insn(bu, 1)
GEN_LX_insn(bu, 2)

typedef uint32_t (*lx_proto_t)(const uint32_t *address, uint32_t offset);

static uint32_t lxw(const uint32_t *address, uint32_t offset, unsigned strd2)
{
	const lx_proto_t fns[3] = { lxw0,  lxw1,  lxw2, };
	return fns[strd2](address, offset);
}

static uint32_t lxh(const uint32_t *address, uint32_t offset, unsigned strd2)
{
	const lx_proto_t fns[3] = { lxh0,  lxh1,  lxh2, };
	return fns[strd2](address, offset);
}

static uint32_t lxhu(const uint32_t *address, uint32_t offset, unsigned strd2)
{
	const lx_proto_t fns[3] = { lxhu0,  lxhu1,  lxhu2, };
	return fns[strd2](address, offset);
}

static uint32_t lxb(const uint32_t *address, uint32_t offset, unsigned strd2)
{
	const lx_proto_t fns[3] = { lxb0,  lxb1,  lxb2, };
	return fns[strd2](address, offset);
}

static uint32_t lxbu(const uint32_t *address, uint32_t offset, unsigned strd2)
{
	const lx_proto_t fns[3] = { lxbu0,  lxbu1,  lxbu2, };
	return fns[strd2](address, offset);
}

#define DATASET_BITS    (16)
#define DATASET_ENTRIES (1U << DATASET_BITS)

static void test_load_lx(int num_passes)
{
	// LX{WHB}<U> shall work with MXU disabled
	mxu_enable(false, false);

	// prepare dataset
	static uint32_t dataset[DATASET_ENTRIES];

	for (unsigned i = 0 ; i < DATASET_ENTRIES; i++)
	    dataset[i] = random32();

	const uint32_t *const dataset32 = (uint32_t*)dataset;
	const uint16_t *const dataset16 = (uint16_t*)dataset;
	const uint8_t *const dataset8 = (uint8_t*)dataset;
	const int16_t *const dataset16s = (int16_t*)dataset;
	const int8_t *const dataset8s = (int8_t*)dataset;

	// test LXW
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % DATASET_ENTRIES;
			int strd2 = random() % 3;
			uint32_t expected = dataset32[offset];
			offset <<= 2 - strd2;
			uint32_t got = lxw(dataset32, offset, strd2);
			if (expected != got) {
				printf("LXW expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(LXW, pass);
	}

	// test LXH
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % (DATASET_ENTRIES * 2);
			int strd2 = random() % 3;
			offset >>= strd2 - 1;
			uint32_t expected = (int32_t)dataset16s[offset << (strd2 - 1)];
			uint32_t got = lxh(dataset32, offset, strd2);
			if (expected != got) {
				printf("LXH expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(LXH, pass);
	}

	// test LXHU
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % (DATASET_ENTRIES * 2);
			int strd2 = random() % 3;
			offset >>= strd2 - 1;
			uint32_t expected = dataset16[offset << (strd2 - 1)];
			uint32_t got = lxhu(dataset32, offset, strd2);
			if (expected != got) {
				printf("LXHU expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(LXHU, pass);
	}

	// test LXB
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % (DATASET_ENTRIES * 4);
			int strd2 = random() % 3;
			offset >>= strd2;
			uint32_t expected = (int32_t)dataset8s[offset << strd2];
			uint32_t got = lxb(dataset32, offset, strd2);
			if (expected != got) {
				printf("LXB expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(LXB, pass);
	}

	// test LXBU
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % (DATASET_ENTRIES * 4);
			int strd2 = random() % 3;
			offset >>= strd2;
			uint32_t expected = dataset8[offset << strd2];
			uint32_t got = lxbu(dataset32, offset, strd2);
			if (expected != got) {
				printf("LXBU expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(LXBU, pass);
	}

	mxu_enable(true, false);
}

#define GEN_S32LDD_insn(name, offset) \
static uint32_t s32ldd##name(const void *base) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32ldd $xr1, %[base], %[offs]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret) \
		: [base] "r"(base), [offs] "i"(offset) \
		: "xr1" \
	); \
	return ret; \
}

GEN_S32LDD_insn(_min_offs, -2048)
GEN_S32LDD_insn(_max_offs, 2044)
GEN_S32LDD_insn(_zero_offs, 0)
GEN_S32LDD_insn(_1024_offs, 1024)
GEN_S32LDD_insn(_m1024_offs, -1024)

typedef uint32_t (*s32ldd_proto_t)(const void *base);
typedef uint32_t (*sxxldd_proto_t)(const void *base, uint32_t initial);

static uint32_t s32ldd(const uint32_t *base, int offset_ptn)
{
	const s32ldd_proto_t fns[5] = { s32ldd_min_offs,  s32ldd_max_offs,
		s32ldd_zero_offs, s32ldd_1024_offs, s32ldd_m1024_offs, };
	return fns[offset_ptn](base);
}

#define GEN_S32LDDR_insn(name, offset) \
static uint32_t s32lddr##name(const void *base) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32lddr $xr1, %[base], %[offs]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret) \
		: [base] "r"(base), [offs] "i"(offset) \
		: "xr1" \
	); \
	return ret; \
}

GEN_S32LDDR_insn(_min_offs, -2048)
GEN_S32LDDR_insn(_max_offs, 2044)
GEN_S32LDDR_insn(_zero_offs, 0)
GEN_S32LDDR_insn(_1024_offs, 1024)
GEN_S32LDDR_insn(_m1024_offs, -1024)

static uint32_t s32lddr(const uint32_t *base, int offset_ptn)
{
	const s32ldd_proto_t fns[5] = { s32lddr_min_offs,  s32lddr_max_offs,
		s32lddr_zero_offs, s32lddr_1024_offs, s32lddr_m1024_offs, };
	return fns[offset_ptn](base);
}

#define GEN_S16LDD_insn(name, offset, optn2) \
static uint32_t s16ldd##name##optn2(const void *base, uint32_t initial) \
{ \
	uint32_t ret = initial; \
	asm volatile ( \
		"s32i2m $xr1, %[ret]\n" \
		"s16ldd $xr1, %[base], %[offs], %[ptn]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "+r"(ret) \
		: [base] "r"(base), [offs] "i"(offset) , [ptn] "i"(optn2) \
		: "xr1" \
	); \
	return ret; \
}

GEN_S16LDD_insn(_min_offs, -512, 0)
GEN_S16LDD_insn(_min_offs, -512, 1)
GEN_S16LDD_insn(_min_offs, -512, 2)
GEN_S16LDD_insn(_min_offs, -512, 3)
GEN_S16LDD_insn(_max_offs, 510, 0)
GEN_S16LDD_insn(_max_offs, 510, 1)
GEN_S16LDD_insn(_max_offs, 510, 2)
GEN_S16LDD_insn(_max_offs, 510, 3)
GEN_S16LDD_insn(_zero_offs, 0, 0)
GEN_S16LDD_insn(_zero_offs, 0, 1)
GEN_S16LDD_insn(_zero_offs, 0, 2)
GEN_S16LDD_insn(_zero_offs, 0, 3)
GEN_S16LDD_insn(_256_offs, 256, 0)
GEN_S16LDD_insn(_256_offs, 256, 1)
GEN_S16LDD_insn(_256_offs, 256, 2)
GEN_S16LDD_insn(_256_offs, 256, 3)
GEN_S16LDD_insn(_m256_offs, -256, 0)
GEN_S16LDD_insn(_m256_offs, -256, 1)
GEN_S16LDD_insn(_m256_offs, -256, 2)
GEN_S16LDD_insn(_m256_offs, -256, 3)

static uint32_t s16ldd(const uint16_t *base, uint32_t initial, int offset_ptn, int optn2)
{
	const sxxldd_proto_t fns[] = {
		s16ldd_min_offs0, s16ldd_min_offs1, s16ldd_min_offs2, s16ldd_min_offs3,
		s16ldd_max_offs0, s16ldd_max_offs1, s16ldd_max_offs2, s16ldd_max_offs3,
		s16ldd_zero_offs0, s16ldd_zero_offs1, s16ldd_zero_offs2, s16ldd_zero_offs3,
		s16ldd_256_offs0, s16ldd_256_offs1, s16ldd_256_offs2, s16ldd_256_offs3,
		s16ldd_m256_offs0, s16ldd_m256_offs1, s16ldd_m256_offs2, s16ldd_m256_offs3,
	};
	return fns[offset_ptn * 4 + optn2](base, initial);
}

#define GEN_S8LDD_insn(name, offset, optn3) \
static uint32_t s8ldd##name##optn3(const void *base, uint32_t initial) \
{ \
	uint32_t ret = initial; \
	asm volatile ( \
		"s32i2m $xr1, %[ret]\n" \
		"s8ldd $xr1, %[base], %[offs], %[ptn]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "+r"(ret) \
		: [base] "r"(base), [offs] "i"(offset) , [ptn] "i"(optn3) \
		: "xr1" \
	); \
	return ret; \
}

GEN_S8LDD_insn(_min_offs, -128, 0)
GEN_S8LDD_insn(_min_offs, -128, 1)
GEN_S8LDD_insn(_min_offs, -128, 2)
GEN_S8LDD_insn(_min_offs, -128, 3)
GEN_S8LDD_insn(_min_offs, -128, 4)
GEN_S8LDD_insn(_min_offs, -128, 5)
GEN_S8LDD_insn(_min_offs, -128, 6)
GEN_S8LDD_insn(_min_offs, -128, 7)
GEN_S8LDD_insn(_max_offs, 127, 0)
GEN_S8LDD_insn(_max_offs, 127, 1)
GEN_S8LDD_insn(_max_offs, 127, 2)
GEN_S8LDD_insn(_max_offs, 127, 3)
GEN_S8LDD_insn(_max_offs, 127, 4)
GEN_S8LDD_insn(_max_offs, 127, 5)
GEN_S8LDD_insn(_max_offs, 127, 6)
GEN_S8LDD_insn(_max_offs, 127, 7)
GEN_S8LDD_insn(_zero_offs, 0, 0)
GEN_S8LDD_insn(_zero_offs, 0, 1)
GEN_S8LDD_insn(_zero_offs, 0, 2)
GEN_S8LDD_insn(_zero_offs, 0, 3)
GEN_S8LDD_insn(_zero_offs, 0, 4)
GEN_S8LDD_insn(_zero_offs, 0, 5)
GEN_S8LDD_insn(_zero_offs, 0, 6)
GEN_S8LDD_insn(_zero_offs, 0, 7)
GEN_S8LDD_insn(_64_offs, 64, 0)
GEN_S8LDD_insn(_64_offs, 64, 1)
GEN_S8LDD_insn(_64_offs, 64, 2)
GEN_S8LDD_insn(_64_offs, 64, 3)
GEN_S8LDD_insn(_64_offs, 64, 4)
GEN_S8LDD_insn(_64_offs, 64, 5)
GEN_S8LDD_insn(_64_offs, 64, 6)
GEN_S8LDD_insn(_64_offs, 64, 7)
GEN_S8LDD_insn(_m64_offs, -64, 0)
GEN_S8LDD_insn(_m64_offs, -64, 1)
GEN_S8LDD_insn(_m64_offs, -64, 2)
GEN_S8LDD_insn(_m64_offs, -64, 3)
GEN_S8LDD_insn(_m64_offs, -64, 4)
GEN_S8LDD_insn(_m64_offs, -64, 5)
GEN_S8LDD_insn(_m64_offs, -64, 6)
GEN_S8LDD_insn(_m64_offs, -64, 7)

static uint32_t s8ldd(const uint8_t *base, uint32_t initial, int offset_ptn, int optn3)
{
	const sxxldd_proto_t fns[] = {
		s8ldd_min_offs0, s8ldd_min_offs1, s8ldd_min_offs2, s8ldd_min_offs3,
		s8ldd_min_offs4, s8ldd_min_offs5, s8ldd_min_offs6, s8ldd_min_offs7,
		s8ldd_max_offs0, s8ldd_max_offs1, s8ldd_max_offs2, s8ldd_max_offs3,
		s8ldd_max_offs4, s8ldd_max_offs5, s8ldd_max_offs6, s8ldd_max_offs7,
		s8ldd_zero_offs0, s8ldd_zero_offs1, s8ldd_zero_offs2, s8ldd_zero_offs3,
		s8ldd_zero_offs4, s8ldd_zero_offs5, s8ldd_zero_offs6, s8ldd_zero_offs7,
		s8ldd_64_offs0, s8ldd_64_offs1, s8ldd_64_offs2, s8ldd_64_offs3,
		s8ldd_64_offs4, s8ldd_64_offs5, s8ldd_64_offs6, s8ldd_64_offs7,
		s8ldd_m64_offs0, s8ldd_m64_offs1, s8ldd_m64_offs2, s8ldd_m64_offs3,
		s8ldd_m64_offs4, s8ldd_m64_offs5, s8ldd_m64_offs6, s8ldd_m64_offs7,
	};
	return fns[offset_ptn * 8 + optn3](base, initial);
}

#define GEN_S32LDI_insn(name, offset) \
static uint32_t s32ldi##name(const void **pbase) \
{ \
	uint32_t ret; \
	const void *base = *pbase; \
	asm volatile ( \
		"s32ldi $xr1, %[base], %[offs]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret), [base] "+r"(base) \
		: [offs] "i"(offset) \
		: "xr1" \
	); \
	*pbase = base; \
	return ret; \
}

GEN_S32LDI_insn(_min_offs, -2048)
GEN_S32LDI_insn(_max_offs, 2044)
GEN_S32LDI_insn(_zero_offs, 0)
GEN_S32LDI_insn(_1024_offs, 1024)
GEN_S32LDI_insn(_m1024_offs, -1024)

typedef uint32_t (*s32ldi_proto_t)(const void **pbase);
typedef uint32_t (*sxxldi_proto_t)(const void **pbase, uint32_t initial);

static uint32_t s32ldi(const void **pbase, int offset_ptn)
{
	const s32ldi_proto_t fns[5] = { s32ldi_min_offs,  s32ldi_max_offs,
		s32ldi_zero_offs, s32ldi_1024_offs, s32ldi_m1024_offs, };
	return fns[offset_ptn](pbase);
}

#define GEN_S32LDIR_insn(name, offset) \
static uint32_t s32ldir##name(const void **pbase) \
{ \
	uint32_t ret; \
	const void *base = *pbase; \
	asm volatile ( \
		"s32ldir $xr1, %[base], %[offs]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "=r"(ret), [base] "+r"(base) \
		: [offs] "i"(offset) \
		: "xr1" \
	); \
	*pbase = base; \
	return ret; \
}

GEN_S32LDIR_insn(_min_offs, -2048)
GEN_S32LDIR_insn(_max_offs, 2044)
GEN_S32LDIR_insn(_zero_offs, 0)
GEN_S32LDIR_insn(_1024_offs, 1024)
GEN_S32LDIR_insn(_m1024_offs, -1024)

static uint32_t s32ldir(const void **pbase, int offset_ptn)
{
	const s32ldi_proto_t fns[5] = { s32ldir_min_offs,  s32ldir_max_offs,
		s32ldir_zero_offs, s32ldir_1024_offs, s32ldir_m1024_offs, };
	return fns[offset_ptn](pbase);
}

#define GEN_S16LDI_insn(name, offset, optn2) \
static uint32_t s16ldi##name##optn2(const void **pbase, uint32_t initial) \
{ \
	uint32_t ret = initial; \
	const void *base = *pbase; \
	asm volatile ( \
		"s32i2m $xr1, %[ret]\n" \
		"s16ldi $xr1, %[base], %[offs], %[ptn]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "+r"(ret), [base] "+r"(base) \
		: [offs] "i"(offset) , [ptn] "i"(optn2) \
		: "xr1" \
	); \
	*pbase = base; \
	return ret; \
}

GEN_S16LDI_insn(_min_offs, -512, 0)
GEN_S16LDI_insn(_min_offs, -512, 1)
GEN_S16LDI_insn(_min_offs, -512, 2)
GEN_S16LDI_insn(_min_offs, -512, 3)
GEN_S16LDI_insn(_max_offs, 510, 0)
GEN_S16LDI_insn(_max_offs, 510, 1)
GEN_S16LDI_insn(_max_offs, 510, 2)
GEN_S16LDI_insn(_max_offs, 510, 3)
GEN_S16LDI_insn(_zero_offs, 0, 0)
GEN_S16LDI_insn(_zero_offs, 0, 1)
GEN_S16LDI_insn(_zero_offs, 0, 2)
GEN_S16LDI_insn(_zero_offs, 0, 3)
GEN_S16LDI_insn(_256_offs, 256, 0)
GEN_S16LDI_insn(_256_offs, 256, 1)
GEN_S16LDI_insn(_256_offs, 256, 2)
GEN_S16LDI_insn(_256_offs, 256, 3)
GEN_S16LDI_insn(_m256_offs, -256, 0)
GEN_S16LDI_insn(_m256_offs, -256, 1)
GEN_S16LDI_insn(_m256_offs, -256, 2)
GEN_S16LDI_insn(_m256_offs, -256, 3)

static uint32_t s16ldi(const void **pbase, uint32_t initial, int offset_ptn, int optn2)
{
	const sxxldi_proto_t fns[] = {
		s16ldi_min_offs0, s16ldi_min_offs1, s16ldi_min_offs2, s16ldi_min_offs3,
		s16ldi_max_offs0, s16ldi_max_offs1, s16ldi_max_offs2, s16ldi_max_offs3,
		s16ldi_zero_offs0, s16ldi_zero_offs1, s16ldi_zero_offs2, s16ldi_zero_offs3,
		s16ldi_256_offs0, s16ldi_256_offs1, s16ldi_256_offs2, s16ldi_256_offs3,
		s16ldi_m256_offs0, s16ldi_m256_offs1, s16ldi_m256_offs2, s16ldi_m256_offs3,
	};
	return fns[offset_ptn * 4 + optn2](pbase, initial);
}

#define GEN_S8LDI_insn(name, offset, optn3) \
static uint32_t s8ldi##name##optn3(const void **pbase, uint32_t initial) \
{ \
	uint32_t ret = initial; \
	const void *base = *pbase; \
	asm volatile ( \
		"s32i2m $xr1, %[ret]\n" \
		"s8ldi $xr1, %[base], %[offs], %[ptn]\n" \
		"s32m2i $xr1, %[ret]\n" \
		: [ret] "+r"(ret), [base] "+r"(base) \
		: [offs] "i"(offset) , [ptn] "i"(optn3) \
		: "xr1" \
	); \
	*pbase = base; \
	return ret; \
}

GEN_S8LDI_insn(_min_offs, -128, 0)
GEN_S8LDI_insn(_min_offs, -128, 1)
GEN_S8LDI_insn(_min_offs, -128, 2)
GEN_S8LDI_insn(_min_offs, -128, 3)
GEN_S8LDI_insn(_min_offs, -128, 4)
GEN_S8LDI_insn(_min_offs, -128, 5)
GEN_S8LDI_insn(_min_offs, -128, 6)
GEN_S8LDI_insn(_min_offs, -128, 7)
GEN_S8LDI_insn(_max_offs, 127, 0)
GEN_S8LDI_insn(_max_offs, 127, 1)
GEN_S8LDI_insn(_max_offs, 127, 2)
GEN_S8LDI_insn(_max_offs, 127, 3)
GEN_S8LDI_insn(_max_offs, 127, 4)
GEN_S8LDI_insn(_max_offs, 127, 5)
GEN_S8LDI_insn(_max_offs, 127, 6)
GEN_S8LDI_insn(_max_offs, 127, 7)
GEN_S8LDI_insn(_zero_offs, 0, 0)
GEN_S8LDI_insn(_zero_offs, 0, 1)
GEN_S8LDI_insn(_zero_offs, 0, 2)
GEN_S8LDI_insn(_zero_offs, 0, 3)
GEN_S8LDI_insn(_zero_offs, 0, 4)
GEN_S8LDI_insn(_zero_offs, 0, 5)
GEN_S8LDI_insn(_zero_offs, 0, 6)
GEN_S8LDI_insn(_zero_offs, 0, 7)
GEN_S8LDI_insn(_64_offs, 64, 0)
GEN_S8LDI_insn(_64_offs, 64, 1)
GEN_S8LDI_insn(_64_offs, 64, 2)
GEN_S8LDI_insn(_64_offs, 64, 3)
GEN_S8LDI_insn(_64_offs, 64, 4)
GEN_S8LDI_insn(_64_offs, 64, 5)
GEN_S8LDI_insn(_64_offs, 64, 6)
GEN_S8LDI_insn(_64_offs, 64, 7)
GEN_S8LDI_insn(_m64_offs, -64, 0)
GEN_S8LDI_insn(_m64_offs, -64, 1)
GEN_S8LDI_insn(_m64_offs, -64, 2)
GEN_S8LDI_insn(_m64_offs, -64, 3)
GEN_S8LDI_insn(_m64_offs, -64, 4)
GEN_S8LDI_insn(_m64_offs, -64, 5)
GEN_S8LDI_insn(_m64_offs, -64, 6)
GEN_S8LDI_insn(_m64_offs, -64, 7)

static uint32_t s8ldi(const void **pbase, uint32_t initial, int offset_ptn, int optn3)
{
	const sxxldi_proto_t fns[] = {
		s8ldi_min_offs0, s8ldi_min_offs1, s8ldi_min_offs2, s8ldi_min_offs3,
		s8ldi_min_offs4, s8ldi_min_offs5, s8ldi_min_offs6, s8ldi_min_offs7,
		s8ldi_max_offs0, s8ldi_max_offs1, s8ldi_max_offs2, s8ldi_max_offs3,
		s8ldi_max_offs4, s8ldi_max_offs5, s8ldi_max_offs6, s8ldi_max_offs7,
		s8ldi_zero_offs0, s8ldi_zero_offs1, s8ldi_zero_offs2, s8ldi_zero_offs3,
		s8ldi_zero_offs4, s8ldi_zero_offs5, s8ldi_zero_offs6, s8ldi_zero_offs7,
		s8ldi_64_offs0, s8ldi_64_offs1, s8ldi_64_offs2, s8ldi_64_offs3,
		s8ldi_64_offs4, s8ldi_64_offs5, s8ldi_64_offs6, s8ldi_64_offs7,
		s8ldi_m64_offs0, s8ldi_m64_offs1, s8ldi_m64_offs2, s8ldi_m64_offs3,
		s8ldi_m64_offs4, s8ldi_m64_offs5, s8ldi_m64_offs6, s8ldi_m64_offs7,
	};
	return fns[offset_ptn * 8 + optn3](pbase, initial);
}

#undef DATASET_BITS
#define DATASET_BITS (10)
static void test_sxxldx(int num_passes)
{
	// prepare dataset
	static uint32_t dataset[DATASET_ENTRIES];

	for (unsigned i = 0 ; i < DATASET_ENTRIES; i++)
	    dataset[i] = random32();

	const uint32_t *const dataset32 = (uint32_t*)&dataset[DATASET_ENTRIES / 2];
	const uint16_t *const dataset16 = (uint16_t*)dataset32;
	const uint8_t *const dataset8 = (uint8_t*)dataset32;

	// test S32LDD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t expected = 0;
			switch (offset_ptn) {
			    case 0: expected = dataset32[-512]; break; // -2048 / 4
			    case 1: expected = dataset32[511]; break; // 2044 / 4
			    case 2: expected = dataset32[0]; break;
			    case 3: expected = dataset32[256]; break;
			    case 4: expected = dataset32[-256]; break;
			}

			uint32_t got = s32ldd(dataset32, offset_ptn);
			if (expected != got) {
				printf("S32LDD expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32LDD, pass);
	}

	// test S32LDDR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t expected = 0;
			switch (offset_ptn) {
			    case 0: expected = __builtin_bswap32(dataset32[-512]); break;
			    case 1: expected = __builtin_bswap32(dataset32[511]); break;
			    case 2: expected = __builtin_bswap32(dataset32[0]); break;
			    case 3: expected = __builtin_bswap32(dataset32[256]); break;
			    case 4: expected = __builtin_bswap32(dataset32[-256]); break;
			}

			uint32_t got = s32lddr(dataset32, offset_ptn);
			if (expected != got) {
				printf("S32LDDR expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32LDDR, pass);
	}

	// test S16LDD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t optn2 = random() % 4;
			uint32_t expected = 0, initial = random32();
			switch (offset_ptn) {
			    case 0: expected = dataset16[-256]; break; // -512 / 2
			    case 1: expected = dataset16[255]; break; // 510 / 2
			    case 2: expected = dataset16[0]; break;
			    case 3: expected = dataset16[128]; break;
			    case 4: expected = dataset16[-128]; break;
			}
			switch (optn2) {
			    case 0: expected = (initial & 0xffff0000) | expected; break;
			    case 1: expected = (initial & 0xffff) | (expected << 16); break;
			    case 2: expected = (int32_t)(int16_t)expected; break;
			    case 3: expected = (expected << 16) | expected; break;
			}

			uint32_t got = s16ldd(dataset16, initial, offset_ptn, optn2);
			if (expected != got) {
				printf("S16LDD expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S16LDD, pass);
	}

	// test S8LDD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t optn3 = random() % 8;
			uint32_t expected = 0, initial = random32();
			switch (offset_ptn) {
			    case 0: expected = dataset8[-128]; break;
			    case 1: expected = dataset8[127]; break;
			    case 2: expected = dataset8[0]; break;
			    case 3: expected = dataset8[64]; break;
			    case 4: expected = dataset8[-64]; break;
			}
			switch (optn3) {
			    case 0: expected = (initial & 0xffffff00) | expected; break;
			    case 1: expected = (initial & 0xffff00ff) | (expected << 8); break;
			    case 2: expected = (initial & 0xff00ffff) | (expected << 16); break;
			    case 3: expected = (initial & 0x00ffffff) | (expected << 24); break;
			    case 4: expected = (expected << 16) | expected; break;
			    case 5: expected = (expected << 24) | (expected << 8); break;
			    case 6: expected = (uint16_t)(int16_t)(int8_t)expected; expected = (expected << 16) | expected; break;
			    case 7: expected = (expected << 24) | (expected << 16) | (expected << 8) | expected; break;
			}

			uint32_t got = s8ldd(dataset8, initial, offset_ptn, optn3);
			if (expected != got) {
				printf("S8LDD expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S8LDD, pass);
	}

	// test S32LDI
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t expected = 0;
			uintptr_t base = (uintptr_t)dataset32;
			uintptr_t expected_base = 0;
			switch (offset_ptn) {
			    case 0: expected = dataset32[-512]; expected_base = base - 2048; break;
			    case 1: expected = dataset32[511]; expected_base = base + 2044; break;
			    case 2: expected = dataset32[0];  expected_base = base; break;
			    case 3: expected = dataset32[256]; expected_base = base + 1024; break;
			    case 4: expected = dataset32[-256]; expected_base = base - 1024; break;
			}

			uint32_t got = s32ldi((const void**)&base, offset_ptn);
			if (expected != got || base != expected_base) {
				printf("S32LDI expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32LDI, pass);
	}

	// test S32LDIR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t expected = 0;
			uintptr_t base = (uintptr_t)dataset32;
			uintptr_t expected_base = 0;
			switch (offset_ptn) {
			    case 0: expected = __builtin_bswap32(dataset32[-512]); expected_base = base - 2048; break;
			    case 1: expected = __builtin_bswap32(dataset32[511]); expected_base = base + 2044; break;
			    case 2: expected = __builtin_bswap32(dataset32[0]);  expected_base = base; break;
			    case 3: expected = __builtin_bswap32(dataset32[256]); expected_base = base + 1024; break;
			    case 4: expected = __builtin_bswap32(dataset32[-256]); expected_base = base - 1024; break;
			}

			uint32_t got = s32ldir((const void**)&base, offset_ptn);
			if (expected != got || base != expected_base) {
				printf("S32LDIR expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32LDIR, pass);
	}

	// test S16LDI
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t optn2 = random() % 4;
			uint32_t expected = 0, initial = random32();
			uintptr_t base = (uintptr_t)dataset16;
			uintptr_t expected_base = 0;
			switch (offset_ptn) {
			    case 0: expected = dataset16[-256]; expected_base = base - 512; break;
			    case 1: expected = dataset16[255]; expected_base = base + 510; break;
			    case 2: expected = dataset16[0]; expected_base = base; break;
			    case 3: expected = dataset16[128]; expected_base = base + 256; break;
			    case 4: expected = dataset16[-128]; expected_base = base - 256; break;
			}
			switch (optn2) {
			    case 0: expected = (initial & 0xffff0000) | expected; break;
			    case 1: expected = (initial & 0xffff) | (expected << 16); break;
			    case 2: expected = (int32_t)(int16_t)expected; break;
			    case 3: expected = (expected << 16) | expected; break;
			}

			uint32_t got = s16ldi((const void**)&base, initial, offset_ptn, optn2);
			if (expected != got || base != expected_base) {
				printf("S16LDI expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S16LDI, pass);
	}

	// test S8LDI
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t optn3 = random() % 8;
			uint32_t expected = 0, initial = random32();
			uintptr_t base = (uintptr_t)dataset8;
			uintptr_t expected_base = 0;
			switch (offset_ptn) {
			    case 0: expected = dataset8[-128]; expected_base = base - 128; break;
			    case 1: expected = dataset8[127]; expected_base = base + 127; break;
			    case 2: expected = dataset8[0]; expected_base = base; break;
			    case 3: expected = dataset8[64]; expected_base = base + 64; break;
			    case 4: expected = dataset8[-64]; expected_base = base - 64; break;
			}
			switch (optn3) {
			    case 0: expected = (initial & 0xffffff00) | expected; break;
			    case 1: expected = (initial & 0xffff00ff) | (expected << 8); break;
			    case 2: expected = (initial & 0xff00ffff) | (expected << 16); break;
			    case 3: expected = (initial & 0x00ffffff) | (expected << 24); break;
			    case 4: expected = (expected << 16) | expected; break;
			    case 5: expected = (expected << 24) | (expected << 8); break;
			    case 6: expected = (uint16_t)(int16_t)(int8_t)expected; expected = (expected << 16) | expected; break;
			    case 7: expected = (expected << 24) | (expected << 16) | (expected << 8) | expected; break;
			}

			uint32_t got = s8ldi((const void**)&base, initial, offset_ptn, optn3);
			if (expected != got || base != expected_base) {
				printf("S8LDI expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S8LDI, pass);
	}
}

#define GEN_S32LDDVx_insn(vr,strd2) \
static uint32_t s32ldd##vr##strd2(const uint32_t *address, uint32_t offset) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32ldd" #vr " $xr1, %[base], %[off], %[shift]\n" \
		"s32m2i $xr1, %[ret]\n" \
	: [ret] "=r"(ret) \
	: [base] "r"(address), [off] "r"(offset), [shift] "i"(strd2) \
	: "xr1"); \
	return ret; \
}

#define GEN_S32LDIVx_insn(vr,strd2) \
static uint32_t s32ldi##vr##strd2(const void **pbase, uint32_t offset) \
{ \
	uint32_t ret; \
	const void *base = *pbase; \
	asm volatile ( \
		"s32ldi" #vr " $xr1, %[base], %[off], %[shift]\n" \
		"s32m2i $xr1, %[ret]\n" \
	: [ret] "=r"(ret), [base] "+r"(base) \
	: [off] "r"(offset), [shift] "i"(strd2) \
	: "xr1"); \
	*pbase = base; \
	return ret; \
}

GEN_S32LDDVx_insn(v, 0)
GEN_S32LDDVx_insn(v, 1)
GEN_S32LDDVx_insn(v, 2)

GEN_S32LDDVx_insn(vr, 0)
GEN_S32LDDVx_insn(vr, 1)
GEN_S32LDDVx_insn(vr, 2)

GEN_S32LDIVx_insn(v, 0)
GEN_S32LDIVx_insn(v, 1)
GEN_S32LDIVx_insn(v, 2)

GEN_S32LDIVx_insn(vr, 0)
GEN_S32LDIVx_insn(vr, 1)
GEN_S32LDIVx_insn(vr, 2)

typedef uint32_t (*s32lddv_proto_t)(const uint32_t *base, uint32_t offset);

static uint32_t s32lddv(const uint32_t *base, uint32_t offset, int strd2)
{
	const s32lddv_proto_t fns[3] = { s32lddv0,  s32lddv1, s32lddv2 };
	return fns[strd2](base, offset);
}

static uint32_t s32lddvr(const uint32_t *base, uint32_t offset, int strd2)
{
	const s32lddv_proto_t fns[3] = { s32lddvr0,  s32lddvr1, s32lddvr2 };
	return fns[strd2](base, offset);
}

typedef uint32_t (*s32ldiv_proto_t)(const void **pbase, uint32_t offset);

static uint32_t s32ldiv(const void **pbase, uint32_t offset, int strd2)
{
	const s32ldiv_proto_t fns[3] = { s32ldiv0,  s32ldiv1, s32ldiv2 };
	return fns[strd2](pbase, offset);
}

static uint32_t s32ldivr(const void **pbase, uint32_t offset, int strd2)
{
	const s32ldiv_proto_t fns[3] = { s32ldivr0,  s32ldivr1, s32ldivr2 };
	return fns[strd2](pbase, offset);
}

#undef DATASET_BITS
#define DATASET_BITS    (16)

static void test_s32ldxvx(int num_passes)
{
	// prepare dataset
	static uint32_t dataset[DATASET_ENTRIES];

	for (unsigned i = 0 ; i < DATASET_ENTRIES; i++)
	    dataset[i] = random32();

	const uint32_t *const dataset32 = (uint32_t*)dataset;

	// test S32LDDV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % DATASET_ENTRIES;
			uint32_t expected = dataset32[offset];
			int strd2 = random() % 3;
			offset <<= (2 - strd2);
			uint32_t got = s32lddv(dataset32, offset, strd2);
			if (expected != got) {
				printf("S32LDDV expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32LDDV, pass);
	}

	// test S32LDDVR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % DATASET_ENTRIES;
			uint32_t expected = __builtin_bswap32(dataset32[offset]);
			int strd2 = random() % 3;
			offset <<= (2 - strd2);
			uint32_t got = s32lddvr(dataset32, offset, strd2);
			if (expected != got) {
				printf("S32LDDVR expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32LDDVR, pass);
	}

	// test S32LDIV
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % DATASET_ENTRIES;
			int strd2 = random() % 3;
			uint32_t expected = dataset32[offset];
			uintptr_t base = (uintptr_t)dataset32;
			uintptr_t expected_base = base + offset * 4;
			offset <<= (2 - strd2);

			uint32_t got = s32ldiv((const void**)&base, offset, strd2);
			if (expected != got || base != expected_base) {
				printf("S32LDIV expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32LDIV, pass);
	}

	// test S32LDIVR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset = random() % DATASET_ENTRIES;
			int strd2 = random() % 3;
			uint32_t expected = __builtin_bswap32(dataset32[offset]);
			uintptr_t base = (uintptr_t)dataset32;
			uintptr_t expected_base = base + offset * 4;
			offset <<= (2 - strd2);

			uint32_t got = s32ldivr((const void**)&base, offset, strd2);
			if (expected != got || base != expected_base) {
				printf("S32LDIVR expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32LDIVR, pass);
	}
}

#define GEN_S32LUI_insn(s8,ptn3) \
static uint32_t s32lui_##ptn3##s8(void) \
{ \
	uint32_t ret; \
	asm volatile ( \
		"s32lui $xr1, %[imm], %[ptn]\n" \
		"s32m2i $xr1, %[ret]\n" \
	: [ret] "=r"(ret) \
	: [imm] "i"(s8), [ptn] "i"(ptn3) \
	: "xr1"); \
	return ret; \
}

#define GEN_S32LUI_all_ptn(s8) \
	GEN_S32LUI_insn(s8,0) \
	GEN_S32LUI_insn(s8,1) \
	GEN_S32LUI_insn(s8,2) \
	GEN_S32LUI_insn(s8,3) \
	GEN_S32LUI_insn(s8,4) \
	GEN_S32LUI_insn(s8,5) \
	GEN_S32LUI_insn(s8,6) \
	GEN_S32LUI_insn(s8,7)

GEN_S32LUI_all_ptn(0)
GEN_S32LUI_all_ptn(1)
GEN_S32LUI_all_ptn(2)
GEN_S32LUI_all_ptn(3)
GEN_S32LUI_all_ptn(4)
GEN_S32LUI_all_ptn(5)
GEN_S32LUI_all_ptn(6)
GEN_S32LUI_all_ptn(7)
GEN_S32LUI_all_ptn(8)
GEN_S32LUI_all_ptn(9)
GEN_S32LUI_all_ptn(10)
GEN_S32LUI_all_ptn(11)
GEN_S32LUI_all_ptn(12)
GEN_S32LUI_all_ptn(13)
GEN_S32LUI_all_ptn(14)
GEN_S32LUI_all_ptn(15)
GEN_S32LUI_all_ptn(16)
GEN_S32LUI_all_ptn(17)
GEN_S32LUI_all_ptn(18)
GEN_S32LUI_all_ptn(19)
GEN_S32LUI_all_ptn(20)
GEN_S32LUI_all_ptn(21)
GEN_S32LUI_all_ptn(22)
GEN_S32LUI_all_ptn(23)
GEN_S32LUI_all_ptn(24)
GEN_S32LUI_all_ptn(25)
GEN_S32LUI_all_ptn(26)
GEN_S32LUI_all_ptn(27)
GEN_S32LUI_all_ptn(28)
GEN_S32LUI_all_ptn(29)
GEN_S32LUI_all_ptn(30)
GEN_S32LUI_all_ptn(31)
GEN_S32LUI_all_ptn(32)
GEN_S32LUI_all_ptn(33)
GEN_S32LUI_all_ptn(34)
GEN_S32LUI_all_ptn(35)
GEN_S32LUI_all_ptn(36)
GEN_S32LUI_all_ptn(37)
GEN_S32LUI_all_ptn(38)
GEN_S32LUI_all_ptn(39)
GEN_S32LUI_all_ptn(40)
GEN_S32LUI_all_ptn(41)
GEN_S32LUI_all_ptn(42)
GEN_S32LUI_all_ptn(43)
GEN_S32LUI_all_ptn(44)
GEN_S32LUI_all_ptn(45)
GEN_S32LUI_all_ptn(46)
GEN_S32LUI_all_ptn(47)
GEN_S32LUI_all_ptn(48)
GEN_S32LUI_all_ptn(49)
GEN_S32LUI_all_ptn(50)
GEN_S32LUI_all_ptn(51)
GEN_S32LUI_all_ptn(52)
GEN_S32LUI_all_ptn(53)
GEN_S32LUI_all_ptn(54)
GEN_S32LUI_all_ptn(55)
GEN_S32LUI_all_ptn(56)
GEN_S32LUI_all_ptn(57)
GEN_S32LUI_all_ptn(58)
GEN_S32LUI_all_ptn(59)
GEN_S32LUI_all_ptn(60)
GEN_S32LUI_all_ptn(61)
GEN_S32LUI_all_ptn(62)
GEN_S32LUI_all_ptn(63)
GEN_S32LUI_all_ptn(64)
GEN_S32LUI_all_ptn(65)
GEN_S32LUI_all_ptn(66)
GEN_S32LUI_all_ptn(67)
GEN_S32LUI_all_ptn(68)
GEN_S32LUI_all_ptn(69)
GEN_S32LUI_all_ptn(70)
GEN_S32LUI_all_ptn(71)
GEN_S32LUI_all_ptn(72)
GEN_S32LUI_all_ptn(73)
GEN_S32LUI_all_ptn(74)
GEN_S32LUI_all_ptn(75)
GEN_S32LUI_all_ptn(76)
GEN_S32LUI_all_ptn(77)
GEN_S32LUI_all_ptn(78)
GEN_S32LUI_all_ptn(79)
GEN_S32LUI_all_ptn(80)
GEN_S32LUI_all_ptn(81)
GEN_S32LUI_all_ptn(82)
GEN_S32LUI_all_ptn(83)
GEN_S32LUI_all_ptn(84)
GEN_S32LUI_all_ptn(85)
GEN_S32LUI_all_ptn(86)
GEN_S32LUI_all_ptn(87)
GEN_S32LUI_all_ptn(88)
GEN_S32LUI_all_ptn(89)
GEN_S32LUI_all_ptn(90)
GEN_S32LUI_all_ptn(91)
GEN_S32LUI_all_ptn(92)
GEN_S32LUI_all_ptn(93)
GEN_S32LUI_all_ptn(94)
GEN_S32LUI_all_ptn(95)
GEN_S32LUI_all_ptn(96)
GEN_S32LUI_all_ptn(97)
GEN_S32LUI_all_ptn(98)
GEN_S32LUI_all_ptn(99)
GEN_S32LUI_all_ptn(100)
GEN_S32LUI_all_ptn(101)
GEN_S32LUI_all_ptn(102)
GEN_S32LUI_all_ptn(103)
GEN_S32LUI_all_ptn(104)
GEN_S32LUI_all_ptn(105)
GEN_S32LUI_all_ptn(106)
GEN_S32LUI_all_ptn(107)
GEN_S32LUI_all_ptn(108)
GEN_S32LUI_all_ptn(109)
GEN_S32LUI_all_ptn(110)
GEN_S32LUI_all_ptn(111)
GEN_S32LUI_all_ptn(112)
GEN_S32LUI_all_ptn(113)
GEN_S32LUI_all_ptn(114)
GEN_S32LUI_all_ptn(115)
GEN_S32LUI_all_ptn(116)
GEN_S32LUI_all_ptn(117)
GEN_S32LUI_all_ptn(118)
GEN_S32LUI_all_ptn(119)
GEN_S32LUI_all_ptn(120)
GEN_S32LUI_all_ptn(121)
GEN_S32LUI_all_ptn(122)
GEN_S32LUI_all_ptn(123)
GEN_S32LUI_all_ptn(124)
GEN_S32LUI_all_ptn(125)
GEN_S32LUI_all_ptn(126)
GEN_S32LUI_all_ptn(127)
GEN_S32LUI_all_ptn(128)
GEN_S32LUI_all_ptn(129)
GEN_S32LUI_all_ptn(130)
GEN_S32LUI_all_ptn(131)
GEN_S32LUI_all_ptn(132)
GEN_S32LUI_all_ptn(133)
GEN_S32LUI_all_ptn(134)
GEN_S32LUI_all_ptn(135)
GEN_S32LUI_all_ptn(136)
GEN_S32LUI_all_ptn(137)
GEN_S32LUI_all_ptn(138)
GEN_S32LUI_all_ptn(139)
GEN_S32LUI_all_ptn(140)
GEN_S32LUI_all_ptn(141)
GEN_S32LUI_all_ptn(142)
GEN_S32LUI_all_ptn(143)
GEN_S32LUI_all_ptn(144)
GEN_S32LUI_all_ptn(145)
GEN_S32LUI_all_ptn(146)
GEN_S32LUI_all_ptn(147)
GEN_S32LUI_all_ptn(148)
GEN_S32LUI_all_ptn(149)
GEN_S32LUI_all_ptn(150)
GEN_S32LUI_all_ptn(151)
GEN_S32LUI_all_ptn(152)
GEN_S32LUI_all_ptn(153)
GEN_S32LUI_all_ptn(154)
GEN_S32LUI_all_ptn(155)
GEN_S32LUI_all_ptn(156)
GEN_S32LUI_all_ptn(157)
GEN_S32LUI_all_ptn(158)
GEN_S32LUI_all_ptn(159)
GEN_S32LUI_all_ptn(160)
GEN_S32LUI_all_ptn(161)
GEN_S32LUI_all_ptn(162)
GEN_S32LUI_all_ptn(163)
GEN_S32LUI_all_ptn(164)
GEN_S32LUI_all_ptn(165)
GEN_S32LUI_all_ptn(166)
GEN_S32LUI_all_ptn(167)
GEN_S32LUI_all_ptn(168)
GEN_S32LUI_all_ptn(169)
GEN_S32LUI_all_ptn(170)
GEN_S32LUI_all_ptn(171)
GEN_S32LUI_all_ptn(172)
GEN_S32LUI_all_ptn(173)
GEN_S32LUI_all_ptn(174)
GEN_S32LUI_all_ptn(175)
GEN_S32LUI_all_ptn(176)
GEN_S32LUI_all_ptn(177)
GEN_S32LUI_all_ptn(178)
GEN_S32LUI_all_ptn(179)
GEN_S32LUI_all_ptn(180)
GEN_S32LUI_all_ptn(181)
GEN_S32LUI_all_ptn(182)
GEN_S32LUI_all_ptn(183)
GEN_S32LUI_all_ptn(184)
GEN_S32LUI_all_ptn(185)
GEN_S32LUI_all_ptn(186)
GEN_S32LUI_all_ptn(187)
GEN_S32LUI_all_ptn(188)
GEN_S32LUI_all_ptn(189)
GEN_S32LUI_all_ptn(190)
GEN_S32LUI_all_ptn(191)
GEN_S32LUI_all_ptn(192)
GEN_S32LUI_all_ptn(193)
GEN_S32LUI_all_ptn(194)
GEN_S32LUI_all_ptn(195)
GEN_S32LUI_all_ptn(196)
GEN_S32LUI_all_ptn(197)
GEN_S32LUI_all_ptn(198)
GEN_S32LUI_all_ptn(199)
GEN_S32LUI_all_ptn(200)
GEN_S32LUI_all_ptn(201)
GEN_S32LUI_all_ptn(202)
GEN_S32LUI_all_ptn(203)
GEN_S32LUI_all_ptn(204)
GEN_S32LUI_all_ptn(205)
GEN_S32LUI_all_ptn(206)
GEN_S32LUI_all_ptn(207)
GEN_S32LUI_all_ptn(208)
GEN_S32LUI_all_ptn(209)
GEN_S32LUI_all_ptn(210)
GEN_S32LUI_all_ptn(211)
GEN_S32LUI_all_ptn(212)
GEN_S32LUI_all_ptn(213)
GEN_S32LUI_all_ptn(214)
GEN_S32LUI_all_ptn(215)
GEN_S32LUI_all_ptn(216)
GEN_S32LUI_all_ptn(217)
GEN_S32LUI_all_ptn(218)
GEN_S32LUI_all_ptn(219)
GEN_S32LUI_all_ptn(220)
GEN_S32LUI_all_ptn(221)
GEN_S32LUI_all_ptn(222)
GEN_S32LUI_all_ptn(223)
GEN_S32LUI_all_ptn(224)
GEN_S32LUI_all_ptn(225)
GEN_S32LUI_all_ptn(226)
GEN_S32LUI_all_ptn(227)
GEN_S32LUI_all_ptn(228)
GEN_S32LUI_all_ptn(229)
GEN_S32LUI_all_ptn(230)
GEN_S32LUI_all_ptn(231)
GEN_S32LUI_all_ptn(232)
GEN_S32LUI_all_ptn(233)
GEN_S32LUI_all_ptn(234)
GEN_S32LUI_all_ptn(235)
GEN_S32LUI_all_ptn(236)
GEN_S32LUI_all_ptn(237)
GEN_S32LUI_all_ptn(238)
GEN_S32LUI_all_ptn(239)
GEN_S32LUI_all_ptn(240)
GEN_S32LUI_all_ptn(241)
GEN_S32LUI_all_ptn(242)
GEN_S32LUI_all_ptn(243)
GEN_S32LUI_all_ptn(244)
GEN_S32LUI_all_ptn(245)
GEN_S32LUI_all_ptn(246)
GEN_S32LUI_all_ptn(247)
GEN_S32LUI_all_ptn(248)
GEN_S32LUI_all_ptn(249)
GEN_S32LUI_all_ptn(250)
GEN_S32LUI_all_ptn(251)
GEN_S32LUI_all_ptn(252)
GEN_S32LUI_all_ptn(253)
GEN_S32LUI_all_ptn(254)
GEN_S32LUI_all_ptn(255)

typedef uint32_t (*s32lui_proto_t)(void);

#define GEN_S32LUI_row(row) { s32lui_0##row, s32lui_1##row, s32lui_2##row, s32lui_3##row, s32lui_4##row, s32lui_5##row, s32lui_6##row, s32lui_7##row, },
static uint32_t s32lui(int s8, int ptn3)
{
	const s32lui_proto_t fns[256][8] = {
		GEN_S32LUI_row(0) GEN_S32LUI_row(1) GEN_S32LUI_row(2) GEN_S32LUI_row(3) GEN_S32LUI_row(4)
		GEN_S32LUI_row(5) GEN_S32LUI_row(6) GEN_S32LUI_row(7) GEN_S32LUI_row(8) GEN_S32LUI_row(9)

		GEN_S32LUI_row(10) GEN_S32LUI_row(11) GEN_S32LUI_row(12) GEN_S32LUI_row(13) GEN_S32LUI_row(14)
		GEN_S32LUI_row(15) GEN_S32LUI_row(16) GEN_S32LUI_row(17) GEN_S32LUI_row(18) GEN_S32LUI_row(19)

		GEN_S32LUI_row(20) GEN_S32LUI_row(21) GEN_S32LUI_row(22) GEN_S32LUI_row(23) GEN_S32LUI_row(24)
		GEN_S32LUI_row(25) GEN_S32LUI_row(26) GEN_S32LUI_row(27) GEN_S32LUI_row(28) GEN_S32LUI_row(29)

		GEN_S32LUI_row(30) GEN_S32LUI_row(31) GEN_S32LUI_row(32) GEN_S32LUI_row(33) GEN_S32LUI_row(34)
		GEN_S32LUI_row(35) GEN_S32LUI_row(36) GEN_S32LUI_row(37) GEN_S32LUI_row(38) GEN_S32LUI_row(39)

		GEN_S32LUI_row(40) GEN_S32LUI_row(41) GEN_S32LUI_row(42) GEN_S32LUI_row(43) GEN_S32LUI_row(44)
		GEN_S32LUI_row(45) GEN_S32LUI_row(46) GEN_S32LUI_row(47) GEN_S32LUI_row(48) GEN_S32LUI_row(49)

		GEN_S32LUI_row(50) GEN_S32LUI_row(51) GEN_S32LUI_row(52) GEN_S32LUI_row(53) GEN_S32LUI_row(54)
		GEN_S32LUI_row(55) GEN_S32LUI_row(56) GEN_S32LUI_row(57) GEN_S32LUI_row(58) GEN_S32LUI_row(59)

		GEN_S32LUI_row(60) GEN_S32LUI_row(61) GEN_S32LUI_row(62) GEN_S32LUI_row(63) GEN_S32LUI_row(64)
		GEN_S32LUI_row(65) GEN_S32LUI_row(66) GEN_S32LUI_row(67) GEN_S32LUI_row(68) GEN_S32LUI_row(69)

		GEN_S32LUI_row(70) GEN_S32LUI_row(71) GEN_S32LUI_row(72) GEN_S32LUI_row(73) GEN_S32LUI_row(74)
		GEN_S32LUI_row(75) GEN_S32LUI_row(76) GEN_S32LUI_row(77) GEN_S32LUI_row(78) GEN_S32LUI_row(79)

		GEN_S32LUI_row(80) GEN_S32LUI_row(81) GEN_S32LUI_row(82) GEN_S32LUI_row(83) GEN_S32LUI_row(84)
		GEN_S32LUI_row(85) GEN_S32LUI_row(86) GEN_S32LUI_row(87) GEN_S32LUI_row(88) GEN_S32LUI_row(89)

		GEN_S32LUI_row(90) GEN_S32LUI_row(91) GEN_S32LUI_row(92) GEN_S32LUI_row(93) GEN_S32LUI_row(94)
		GEN_S32LUI_row(95) GEN_S32LUI_row(96) GEN_S32LUI_row(97) GEN_S32LUI_row(98) GEN_S32LUI_row(99)

		GEN_S32LUI_row(100) GEN_S32LUI_row(101) GEN_S32LUI_row(102) GEN_S32LUI_row(103) GEN_S32LUI_row(104)
		GEN_S32LUI_row(105) GEN_S32LUI_row(106) GEN_S32LUI_row(107) GEN_S32LUI_row(108) GEN_S32LUI_row(109)

		GEN_S32LUI_row(110) GEN_S32LUI_row(111) GEN_S32LUI_row(112) GEN_S32LUI_row(113) GEN_S32LUI_row(114)
		GEN_S32LUI_row(115) GEN_S32LUI_row(116) GEN_S32LUI_row(117) GEN_S32LUI_row(118) GEN_S32LUI_row(119)

		GEN_S32LUI_row(120) GEN_S32LUI_row(121) GEN_S32LUI_row(122) GEN_S32LUI_row(123) GEN_S32LUI_row(124)
		GEN_S32LUI_row(125) GEN_S32LUI_row(126) GEN_S32LUI_row(127) GEN_S32LUI_row(128) GEN_S32LUI_row(129)

		GEN_S32LUI_row(130) GEN_S32LUI_row(131) GEN_S32LUI_row(132) GEN_S32LUI_row(133) GEN_S32LUI_row(134)
		GEN_S32LUI_row(135) GEN_S32LUI_row(136) GEN_S32LUI_row(137) GEN_S32LUI_row(138) GEN_S32LUI_row(139)

		GEN_S32LUI_row(140) GEN_S32LUI_row(141) GEN_S32LUI_row(142) GEN_S32LUI_row(143) GEN_S32LUI_row(144)
		GEN_S32LUI_row(145) GEN_S32LUI_row(146) GEN_S32LUI_row(147) GEN_S32LUI_row(148) GEN_S32LUI_row(149)

		GEN_S32LUI_row(150) GEN_S32LUI_row(151) GEN_S32LUI_row(152) GEN_S32LUI_row(153) GEN_S32LUI_row(154)
		GEN_S32LUI_row(155) GEN_S32LUI_row(156) GEN_S32LUI_row(157) GEN_S32LUI_row(158) GEN_S32LUI_row(159)

		GEN_S32LUI_row(160) GEN_S32LUI_row(161) GEN_S32LUI_row(162) GEN_S32LUI_row(163) GEN_S32LUI_row(164)
		GEN_S32LUI_row(165) GEN_S32LUI_row(166) GEN_S32LUI_row(167) GEN_S32LUI_row(168) GEN_S32LUI_row(169)

		GEN_S32LUI_row(170) GEN_S32LUI_row(171) GEN_S32LUI_row(172) GEN_S32LUI_row(173) GEN_S32LUI_row(174)
		GEN_S32LUI_row(175) GEN_S32LUI_row(176) GEN_S32LUI_row(177) GEN_S32LUI_row(178) GEN_S32LUI_row(179)

		GEN_S32LUI_row(180) GEN_S32LUI_row(181) GEN_S32LUI_row(182) GEN_S32LUI_row(183) GEN_S32LUI_row(184)
		GEN_S32LUI_row(185) GEN_S32LUI_row(186) GEN_S32LUI_row(187) GEN_S32LUI_row(188) GEN_S32LUI_row(189)

		GEN_S32LUI_row(190) GEN_S32LUI_row(191) GEN_S32LUI_row(192) GEN_S32LUI_row(193) GEN_S32LUI_row(194)
		GEN_S32LUI_row(195) GEN_S32LUI_row(196) GEN_S32LUI_row(197) GEN_S32LUI_row(198) GEN_S32LUI_row(199)

		GEN_S32LUI_row(200) GEN_S32LUI_row(201) GEN_S32LUI_row(202) GEN_S32LUI_row(203) GEN_S32LUI_row(204)
		GEN_S32LUI_row(205) GEN_S32LUI_row(206) GEN_S32LUI_row(207) GEN_S32LUI_row(208) GEN_S32LUI_row(209)

		GEN_S32LUI_row(210) GEN_S32LUI_row(211) GEN_S32LUI_row(212) GEN_S32LUI_row(213) GEN_S32LUI_row(214)
		GEN_S32LUI_row(215) GEN_S32LUI_row(216) GEN_S32LUI_row(217) GEN_S32LUI_row(218) GEN_S32LUI_row(219)

		GEN_S32LUI_row(220) GEN_S32LUI_row(221) GEN_S32LUI_row(222) GEN_S32LUI_row(223) GEN_S32LUI_row(224)
		GEN_S32LUI_row(225) GEN_S32LUI_row(226) GEN_S32LUI_row(227) GEN_S32LUI_row(228) GEN_S32LUI_row(229)

		GEN_S32LUI_row(230) GEN_S32LUI_row(231) GEN_S32LUI_row(232) GEN_S32LUI_row(233) GEN_S32LUI_row(234)
		GEN_S32LUI_row(235) GEN_S32LUI_row(236) GEN_S32LUI_row(237) GEN_S32LUI_row(238) GEN_S32LUI_row(239)

		GEN_S32LUI_row(240) GEN_S32LUI_row(241) GEN_S32LUI_row(242) GEN_S32LUI_row(243) GEN_S32LUI_row(244)
		GEN_S32LUI_row(245) GEN_S32LUI_row(246) GEN_S32LUI_row(247) GEN_S32LUI_row(248) GEN_S32LUI_row(249)

		GEN_S32LUI_row(250) GEN_S32LUI_row(251) GEN_S32LUI_row(252) GEN_S32LUI_row(253) GEN_S32LUI_row(254)
		GEN_S32LUI_row(255)
	};
	return fns[s8][ptn3]();
}

static void test_s32lui(void)
{
	bool pass = true;
	for (int s8 = 0 ; s8 < 256 && pass; s8++)
	{
		for (int ptn3 = 0 ; ptn3 < 8; ptn3++)
		{
			uint32_t expected = 0;
			switch (ptn3) {
			case 0: expected = s8; break;
			case 1: expected = s8 << 8; break;
			case 2: expected = s8 << 16; break;
			case 3: expected = s8 << 24; break;
			case 4: expected = (s8 << 16) | s8; break;
			case 5: expected = (s8 << 24) | (s8 << 8); break;
			case 6: { uint16_t s16 = (int16_t)(int8_t)s8; expected = (s16 << 16) | s16; break;}
			case 7: expected = (s8 << 24) | (s8 << 16) | (s8 << 8) | (s8 << 0); break;
			}
			uint32_t got = s32lui(s8, ptn3);
			if (got != expected) {
				printf("S32LUI expected: %04x got: %04x\n", expected, got);
				pass = false;
				break;
			}
		}
	}
	PRINT_TEST_RESULT(S32LUI, pass);
}

void test_loads(int num_passes)
{
	test_load_lx(num_passes);
	test_sxxldx(num_passes);
	test_s32ldxvx(num_passes);
	test_s32lui();
}






#define GEN_S32STD_insn(name, offset) \
static void s32std##name(uint32_t *base, uint32_t val) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[val]\n" \
		"s32std $xr1, %[base], %[offs]\n" \
		: \
		: [val] "r"(val), [base] "r"(base), [offs] "i"(offset) \
		: "xr1", "memory" \
	); \
}

GEN_S32STD_insn(_min_offs, -2048)
GEN_S32STD_insn(_max_offs, 2044)
GEN_S32STD_insn(_zero_offs, 0)
GEN_S32STD_insn(_1024_offs, 1024)
GEN_S32STD_insn(_m1024_offs, -1024)

typedef void (*s32std_proto_t)(uint32_t *base, uint32_t val);
typedef void (*s16std_proto_t)(uint16_t *base, uint32_t val);
typedef void (*s8std_proto_t)(uint8_t *base, uint32_t val);

static void s32std(uint32_t *base, uint32_t val, int offset_ptn)
{
	const s32std_proto_t fns[5] = { s32std_min_offs,  s32std_max_offs,
		s32std_zero_offs, s32std_1024_offs, s32std_m1024_offs, };
	fns[offset_ptn](base, val);
}

#define GEN_S32STDR_insn(name, offset) \
static void s32stdr##name(uint32_t *base, uint32_t val) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[val]\n" \
		"s32stdr $xr1, %[base], %[offs]\n" \
		: \
		: [val] "r"(val), [base] "r"(base), [offs] "i"(offset) \
		: "xr1", "memory" \
	); \
}

GEN_S32STDR_insn(_min_offs, -2048)
GEN_S32STDR_insn(_max_offs, 2044)
GEN_S32STDR_insn(_zero_offs, 0)
GEN_S32STDR_insn(_1024_offs, 1024)
GEN_S32STDR_insn(_m1024_offs, -1024)

static void s32stdr(uint32_t *base, uint32_t val, int offset_ptn)
{
	const s32std_proto_t fns[5] = { s32stdr_min_offs,  s32stdr_max_offs,
		s32stdr_zero_offs, s32stdr_1024_offs, s32stdr_m1024_offs, };
	fns[offset_ptn](base, val);
}

#define GEN_S16STD_insn(name, offset, optn2) \
static void s16std##name##optn2(uint16_t *base, uint32_t val) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[val]\n" \
		"s16std $xr1, %[base], %[offs], %[ptn]\n" \
		: \
		: [val] "r"(val), [base] "r"(base), [offs] "i"(offset), [ptn] "i"(optn2) \
		: "xr1", "memory" \
	); \
}

GEN_S16STD_insn(_min_offs, -512, 0)
GEN_S16STD_insn(_min_offs, -512, 1)
GEN_S16STD_insn(_max_offs, 510, 0)
GEN_S16STD_insn(_max_offs, 510, 1)
GEN_S16STD_insn(_zero_offs, 0, 0)
GEN_S16STD_insn(_zero_offs, 0, 1)
GEN_S16STD_insn(_256_offs, 256, 0)
GEN_S16STD_insn(_256_offs, 256, 1)
GEN_S16STD_insn(_m256_offs, -256, 0)
GEN_S16STD_insn(_m256_offs, -256, 1)

static void s16std(uint16_t *base, uint32_t val, int offset_ptn, int optn2)
{
	const s16std_proto_t fns[] = {
		s16std_min_offs0, s16std_min_offs1,
		s16std_max_offs0, s16std_max_offs1,
		s16std_zero_offs0, s16std_zero_offs1,
		s16std_256_offs0, s16std_256_offs1,
		s16std_m256_offs0, s16std_m256_offs1,
	};
	fns[offset_ptn * 2 + optn2](base, val);
}

#define GEN_S8STD_insn(name, offset, optn3) \
static void s8std##name##optn3(uint8_t *base, uint32_t val) \
{ \
	asm volatile ( \
		"s32i2m $xr1, %[val]\n" \
		"s8std $xr1, %[base], %[offs], %[ptn]\n" \
		: \
		: [val] "r"(val), [base] "r"(base), [offs] "i"(offset), [ptn] "i"(optn3) \
		: "xr1", "memory" \
	); \
}

GEN_S8STD_insn(_min_offs, -128, 0)
GEN_S8STD_insn(_min_offs, -128, 1)
GEN_S8STD_insn(_min_offs, -128, 2)
GEN_S8STD_insn(_min_offs, -128, 3)
GEN_S8STD_insn(_max_offs, 127, 0)
GEN_S8STD_insn(_max_offs, 127, 1)
GEN_S8STD_insn(_max_offs, 127, 2)
GEN_S8STD_insn(_max_offs, 127, 3)
GEN_S8STD_insn(_zero_offs, 0, 0)
GEN_S8STD_insn(_zero_offs, 0, 1)
GEN_S8STD_insn(_zero_offs, 0, 2)
GEN_S8STD_insn(_zero_offs, 0, 3)
GEN_S8STD_insn(_64_offs, 64, 0)
GEN_S8STD_insn(_64_offs, 64, 1)
GEN_S8STD_insn(_64_offs, 64, 2)
GEN_S8STD_insn(_64_offs, 64, 3)
GEN_S8STD_insn(_m64_offs, -64, 0)
GEN_S8STD_insn(_m64_offs, -64, 1)
GEN_S8STD_insn(_m64_offs, -64, 2)
GEN_S8STD_insn(_m64_offs, -64, 3)

static void s8std(uint8_t *base, uint32_t initial, int offset_ptn, int optn3)
{
	const s8std_proto_t fns[] = {
		s8std_min_offs0, s8std_min_offs1, s8std_min_offs2, s8std_min_offs3,
		s8std_max_offs0, s8std_max_offs1, s8std_max_offs2, s8std_max_offs3,
		s8std_zero_offs0, s8std_zero_offs1, s8std_zero_offs2, s8std_zero_offs3,
		s8std_64_offs0, s8std_64_offs1, s8std_64_offs2, s8std_64_offs3,
		s8std_m64_offs0, s8std_m64_offs1, s8std_m64_offs2, s8std_m64_offs3,
	};
	fns[offset_ptn * 4 + optn3](base, initial);
}

#define GEN_S32SDI_insn(name, offset) \
static void s32sdi##name(uint32_t **pbase, uint32_t val) \
{ \
	uint32_t *base = *pbase; \
	asm volatile ( \
		"s32i2m $xr1, %[val]\n" \
		"s32sdi $xr1, %[base], %[offs]\n" \
		: [base] "+r"(base) \
		: [val] "r"(val), [offs] "i"(offset) \
		: "xr1", "memory" \
	); \
	*pbase = base; \
}

GEN_S32SDI_insn(_min_offs, -2048)
GEN_S32SDI_insn(_max_offs, 2044)
GEN_S32SDI_insn(_zero_offs, 0)
GEN_S32SDI_insn(_1024_offs, 1024)
GEN_S32SDI_insn(_m1024_offs, -1024)

typedef void (*s32sdi_proto_t)(uint32_t **pbase, uint32_t val);
typedef void (*s16sdi_proto_t)(uint16_t **pbase, uint32_t val);
typedef void (*s8sdi_proto_t)(uint8_t **pbase, uint32_t val);

static void s32sdi(uint32_t **pbase, uint32_t val, int offset_ptn)
{
	const s32sdi_proto_t fns[5] = { s32sdi_min_offs,  s32sdi_max_offs,
		s32sdi_zero_offs, s32sdi_1024_offs, s32sdi_m1024_offs, };
	fns[offset_ptn](pbase, val);
}

#define GEN_S32SDIR_insn(name, offset) \
static void s32sdir##name(uint32_t **pbase, uint32_t val) \
{ \
	uint32_t *base = *pbase; \
	asm volatile ( \
		"s32i2m $xr1, %[val]\n" \
		"s32sdir $xr1, %[base], %[offs]\n" \
		: [base] "+r"(base) \
		: [val] "r"(val), [offs] "i"(offset) \
		: "xr1", "memory" \
	); \
	*pbase = base; \
}

GEN_S32SDIR_insn(_min_offs, -2048)
GEN_S32SDIR_insn(_max_offs, 2044)
GEN_S32SDIR_insn(_zero_offs, 0)
GEN_S32SDIR_insn(_1024_offs, 1024)
GEN_S32SDIR_insn(_m1024_offs, -1024)

static void s32sdir(uint32_t **pbase, uint32_t val, int offset_ptn)
{
	const s32sdi_proto_t fns[5] = { s32sdir_min_offs,  s32sdir_max_offs,
		s32sdir_zero_offs, s32sdir_1024_offs, s32sdir_m1024_offs, };
	fns[offset_ptn](pbase, val);
}

#define GEN_S16SDI_insn(name, offset, optn2) \
static void s16sdi##name##optn2(uint16_t **pbase, uint32_t val) \
{ \
	uint16_t *base = *pbase; \
	asm volatile ( \
		"s32i2m $xr1, %[val]\n" \
		"s16sdi $xr1, %[base], %[offs], %[ptn]\n" \
		: [base] "+r"(base) \
		: [val] "r"(val), [offs] "i"(offset), [ptn] "i"(optn2) \
		: "xr1", "memory" \
	); \
	*pbase = base; \
}

GEN_S16SDI_insn(_min_offs, -512, 0)
GEN_S16SDI_insn(_min_offs, -512, 1)
GEN_S16SDI_insn(_max_offs, 510, 0)
GEN_S16SDI_insn(_max_offs, 510, 1)
GEN_S16SDI_insn(_zero_offs, 0, 0)
GEN_S16SDI_insn(_zero_offs, 0, 1)
GEN_S16SDI_insn(_256_offs, 256, 0)
GEN_S16SDI_insn(_256_offs, 256, 1)
GEN_S16SDI_insn(_m256_offs, -256, 0)
GEN_S16SDI_insn(_m256_offs, -256, 1)

static void s16sdi(uint16_t **pbase, uint32_t val, int offset_ptn, int optn2)
{
	const s16sdi_proto_t fns[] = {
		s16sdi_min_offs0, s16sdi_min_offs1,
		s16sdi_max_offs0, s16sdi_max_offs1,
		s16sdi_zero_offs0, s16sdi_zero_offs1,
		s16sdi_256_offs0, s16sdi_256_offs1,
		s16sdi_m256_offs0, s16sdi_m256_offs1,
	};
	fns[offset_ptn * 2 + optn2](pbase, val);
}

#define GEN_S8SDI_insn(name, offset, optn3) \
static void s8sdi##name##optn3(uint8_t **pbase, uint32_t val) \
{ \
	uint8_t *base = *pbase; \
	asm volatile ( \
		"s32i2m $xr1, %[val]\n" \
		"s8sdi $xr1, %[base], %[offs], %[ptn]\n" \
		: [base] "+r"(base) \
		: [val] "r"(val), [offs] "i"(offset), [ptn] "i"(optn3) \
		: "xr1", "memory" \
	); \
	*pbase = base; \
}

GEN_S8SDI_insn(_min_offs, -128, 0)
GEN_S8SDI_insn(_min_offs, -128, 1)
GEN_S8SDI_insn(_min_offs, -128, 2)
GEN_S8SDI_insn(_min_offs, -128, 3)
GEN_S8SDI_insn(_max_offs, 127, 0)
GEN_S8SDI_insn(_max_offs, 127, 1)
GEN_S8SDI_insn(_max_offs, 127, 2)
GEN_S8SDI_insn(_max_offs, 127, 3)
GEN_S8SDI_insn(_zero_offs, 0, 0)
GEN_S8SDI_insn(_zero_offs, 0, 1)
GEN_S8SDI_insn(_zero_offs, 0, 2)
GEN_S8SDI_insn(_zero_offs, 0, 3)
GEN_S8SDI_insn(_64_offs, 64, 0)
GEN_S8SDI_insn(_64_offs, 64, 1)
GEN_S8SDI_insn(_64_offs, 64, 2)
GEN_S8SDI_insn(_64_offs, 64, 3)
GEN_S8SDI_insn(_m64_offs, -64, 0)
GEN_S8SDI_insn(_m64_offs, -64, 1)
GEN_S8SDI_insn(_m64_offs, -64, 2)
GEN_S8SDI_insn(_m64_offs, -64, 3)

static void s8sdi(uint8_t **pbase, uint32_t val, int offset_ptn, int optn3)
{
	const s8sdi_proto_t fns[] = {
		s8sdi_min_offs0, s8sdi_min_offs1, s8sdi_min_offs2, s8sdi_min_offs3,
		s8sdi_max_offs0, s8sdi_max_offs1, s8sdi_max_offs2, s8sdi_max_offs3,
		s8sdi_zero_offs0, s8sdi_zero_offs1, s8sdi_zero_offs2, s8sdi_zero_offs3,
		s8sdi_64_offs0, s8sdi_64_offs1, s8sdi_64_offs2, s8sdi_64_offs3,
		s8sdi_m64_offs0, s8sdi_m64_offs1, s8sdi_m64_offs2, s8sdi_m64_offs3,
	};
	fns[offset_ptn * 4 + optn3](pbase, val);
}

#undef DATASET_BITS
#define DATASET_BITS (10)
static void test_sxxstdx(int num_passes)
{
	// prepare dataset
	static uint32_t dataset[DATASET_ENTRIES];

	uint32_t *const dataset32 = (uint32_t*)&dataset[DATASET_ENTRIES / 2];
	uint16_t *const dataset16 = (uint16_t*)dataset32;
	uint8_t *const dataset8 = (uint8_t*)dataset32;

	// test S32STD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t expected, val = random32();
			expected = val;

			s32std(dataset32, val, offset_ptn);
			uint32_t got = 0;
			switch (offset_ptn) {
			    case 0: got = dataset32[-512]; break; // -2048 / 4
			    case 1: got = dataset32[511]; break; // 2044 / 4
			    case 2: got = dataset32[0]; break;
			    case 3: got = dataset32[256]; break;
			    case 4: got = dataset32[-256]; break;
			}
			if (expected != got) {
				printf("S32STD expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32STD, pass);
	}

	// test S32STDR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t expected, val = random32();
			expected = __builtin_bswap32(val);

			s32stdr(dataset32, val, offset_ptn);
			uint32_t got = 0;
			switch (offset_ptn) {
			    case 0: got = dataset32[-512]; break; // -2048 / 4
			    case 1: got = dataset32[511]; break; // 2044 / 4
			    case 2: got = dataset32[0]; break;
			    case 3: got = dataset32[256]; break;
			    case 4: got = dataset32[-256]; break;
			}
			if (expected != got) {
				printf("S32STDR expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32STDR, pass);
	}

	// test S16STD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t optn2 = random() % 2;
			uint32_t expected = 0, val = random32();
			switch (optn2) {
			    case 0: expected = val & 0xffff; break;
			    case 1: expected = val >> 16; break;
			}

			s16std(dataset16, val, offset_ptn, optn2);
			uint32_t got = 0;
			switch (offset_ptn) {
			    case 0: got = dataset16[-256]; break; // -512 / 2
			    case 1: got = dataset16[255]; break; // 510 / 2
			    case 2: got = dataset16[0]; break;
			    case 3: got = dataset16[128]; break;
			    case 4: got = dataset16[-128]; break;
			}
			if (expected != got) {
				printf("S16STD expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S16STD, pass);
	}

	// test S8STD
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t optn3 = random() % 4;
			uint32_t expected = 0, val = random32();
			switch (optn3) {
			    case 0: expected = val & 0xff; break;
			    case 1: expected = (val >> 8) & 0xff; break;
			    case 2: expected = (val >> 16) & 0xff; break;
			    case 3: expected = (val >> 24) & 0xff; break;
			}

			s8std(dataset8, val, offset_ptn, optn3);
			uint32_t got = 0;
			switch (offset_ptn) {
			    case 0: got = dataset8[-128]; break;
			    case 1: got = dataset8[127]; break;
			    case 2: got = dataset8[0]; break;
			    case 3: got = dataset8[64]; break;
			    case 4: got = dataset8[-64]; break;
			}
			if (expected != got) {
				printf("S8STD expected: %08x got: %08x\n", expected, got);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S8STD, pass);
	}

	// test S32SDI
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t val = random32();
			uint32_t expected = val;
			uintptr_t base = (uintptr_t)dataset32;
			uintptr_t expected_base = 0;
			switch (offset_ptn) {
			    case 0: expected_base = base - 2048; break;
			    case 1: expected_base = base + 2044; break;
			    case 2: expected_base = base; break;
			    case 3: expected_base = base + 1024; break;
			    case 4: expected_base = base - 1024; break;
			}

			s32sdi((uint32_t**)&base, val, offset_ptn);
			uint32_t got = 0;
			switch (offset_ptn) {
			    case 0: got = dataset32[-512]; break;
			    case 1: got = dataset32[511]; break;
			    case 2: got = dataset32[0]; break;
			    case 3: got = dataset32[256]; break;
			    case 4: got = dataset32[-256]; break;
			}
			if (expected != got || base != expected_base) {
				printf("S32SDI expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32SDI, pass);
	}

	// test S32SDIR
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t val = random32();
			uint32_t expected = __builtin_bswap32(val);
			uintptr_t base = (uintptr_t)dataset32;
			uintptr_t expected_base = 0;
			switch (offset_ptn) {
			    case 0: expected_base = base - 2048; break;
			    case 1: expected_base = base + 2044; break;
			    case 2: expected_base = base; break;
			    case 3: expected_base = base + 1024; break;
			    case 4: expected_base = base - 1024; break;
			}

			s32sdir((uint32_t**)&base, val, offset_ptn);
			uint32_t got = 0;
			switch (offset_ptn) {
			    case 0: got = dataset32[-512]; break;
			    case 1: got = dataset32[511]; break;
			    case 2: got = dataset32[0]; break;
			    case 3: got = dataset32[256]; break;
			    case 4: got = dataset32[-256]; break;
			}
			if (expected != got || base != expected_base) {
				printf("S32SDIR expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S32SDIR, pass);
	}

	// test S16SDI
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t optn2 = random() % 2;
			uint32_t val = random32();
			uint32_t expected = 0;
			uintptr_t base = (uintptr_t)dataset16;
			uintptr_t expected_base = 0;
			switch (offset_ptn) {
			    case 0: expected_base = base - 512; break;
			    case 1: expected_base = base + 510; break;
			    case 2: expected_base = base; break;
			    case 3: expected_base = base + 256; break;
			    case 4: expected_base = base - 256; break;
			}
			switch (optn2) {
			    case 0: expected = val & 0xffff; break;
			    case 1: expected = val >> 16; break;
			}

			s16sdi((uint16_t**)&base, val, offset_ptn, optn2);
			uint32_t got = 0;
			switch (offset_ptn) {
			    case 0: got = dataset16[-256]; break;
			    case 1: got = dataset16[255]; break;
			    case 2: got = dataset16[0]; break;
			    case 3: got = dataset16[128]; break;
			    case 4: got = dataset16[-128]; break;
			}
			if (expected != got || base != expected_base) {
				printf("S16SDI expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S16SDI, pass);
	}

	// test S8SDI
	{
		bool pass = true;
		for (int i = 0; i < num_passes; i++)
		{
			uint32_t offset_ptn = random() % 5;
			uint32_t optn3 = random() % 4;
			uint32_t val = random32();
			uint32_t expected = 0;
			uintptr_t base = (uintptr_t)dataset8;
			uintptr_t expected_base = 0;
			switch (offset_ptn) {
			    case 0: expected_base = base - 128; break;
			    case 1: expected_base = base + 127; break;
			    case 2: expected_base = base; break;
			    case 3: expected_base = base + 64; break;
			    case 4: expected_base = base - 64; break;
			}
			switch (optn3) {
			    case 0: expected = val & 0xff; break;
			    case 1: expected = (val >> 8) & 0xff; break;
			    case 2: expected = (val >> 16) & 0xff; break;
			    case 3: expected = (val >> 24) & 0xff; break;
			}

			s8sdi((uint8_t**)&base, val, offset_ptn, optn3);
			uint32_t got = 0;
			switch (offset_ptn) {
			    case 0: got = dataset8[-128]; break;
			    case 1: got = dataset8[127]; break;
			    case 2: got = dataset8[0]; break;
			    case 3: got = dataset8[64]; break;
			    case 4: got = dataset8[-64]; break;
			}
			if (expected != got || base != expected_base) {
				printf("S8SDI expected: %08x got: %08x, addresses: %08x %08x\n",
				       expected, got, base, expected_base);
				pass = false;
				break;
			}
		}
		PRINT_TEST_RESULT(S8SDI, pass);
	}
}


void test_stores(int num_passes)
{
	test_sxxstdx(num_passes);
}
