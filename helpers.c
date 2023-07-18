#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "helpers.h"

uint32_t s32m2i(int reg)
{
#define GEN_ASM(from) \
	asm volatile ("s32m2i $" from ", %[GPR]\n" : [GPR] "=r"(ret) : : );

	uint32_t ret;

	switch (reg) {
	case 0: GEN_ASM("xr0"); break;
	case 1: GEN_ASM("xr1"); break;
	case 2: GEN_ASM("xr2"); break;
	case 3: GEN_ASM("xr3"); break;
	case 4: GEN_ASM("xr4"); break;
	case 5: GEN_ASM("xr5"); break;
	case 6: GEN_ASM("xr6"); break;
	case 7: GEN_ASM("xr7"); break;
	case 8: GEN_ASM("xr8"); break;
	case 9: GEN_ASM("xr9"); break;
	case 10: GEN_ASM("xr10"); break;
	case 11: GEN_ASM("xr11"); break;
	case 12: GEN_ASM("xr12"); break;
	case 13: GEN_ASM("xr13"); break;
	case 14: GEN_ASM("xr14"); break;
	case 15: GEN_ASM("xr15"); break;
	case 16: GEN_ASM("xr16"); break;
	default: abort();
	}
	return ret;
#undef GEN_ASM
}

void s32i2m(int reg, uint32_t val)
{
#define GEN_ASM(to) \
	asm volatile ("s32i2m $" to ", %[from]\n" : : [from] "r"(val) : to);

	switch (reg) {
	case 0: GEN_ASM("xr0"); break;
	case 1: GEN_ASM("xr1"); break;
	case 2: GEN_ASM("xr2"); break;
	case 3: GEN_ASM("xr3"); break;
	case 4: GEN_ASM("xr4"); break;
	case 5: GEN_ASM("xr5"); break;
	case 6: GEN_ASM("xr6"); break;
	case 7: GEN_ASM("xr7"); break;
	case 8: GEN_ASM("xr8"); break;
	case 9: GEN_ASM("xr9"); break;
	case 10: GEN_ASM("xr10"); break;
	case 11: GEN_ASM("xr11"); break;
	case 12: GEN_ASM("xr12"); break;
	case 13: GEN_ASM("xr13"); break;
	case 14: GEN_ASM("xr14"); break;
	case 15: GEN_ASM("xr15"); break;
	case 16: GEN_ASM("xr16"); break;
	default: abort();
	}
#undef GEN_ASM
}

bool mxu_enable(bool enable, bool quiet)
{
	uint32_t state = s32m2i(16);
	if (!!(state & 1) == enable) {
		if (!quiet)
			printf("MXU already %sabled\n", enable ? "en" : "dis");
		return true;
	}
	if (enable)
		state |= 1u;
	else
		state &= ~1u;
	s32i2m(16, state);
	asm volatile ("nop\nnop\nnop\n");
	uint32_t expected = state;
	state = s32m2i(16);
	if (state != expected && !quiet)
		printf("MXU %sable failed: expected 0x%08x, got 0x%08x\n", enable ? "en" : "dis", expected, state);
	return state == expected;
}

uint32_t random32(void)
{
	return (uint32_t)random() << 16
		| ((uint32_t)random() & 0xffff);
}

uint64_t random64(void)
{
	return (uint64_t)random() << 40
		| (((uint64_t)random() << 16) & 0xffffff0000LLU)
		| ((uint64_t)random() & 0xffff);
}
