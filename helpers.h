#include <stdint.h>
#include <stdbool.h>

/**
 * @brief executes MXU s32m2i instruction
 * @param reg MXU register to retrieve (0..16)
 * @return current value of the MXU register
 */
uint32_t s32m2i(int reg);

/**
 * @brief executes MXU s32i2m instruction
 * @param reg MXU register to set  (0..16)
 * @param val a new value of the MXU register
 */
void s32i2m(int reg, uint32_t val);

/**
 * @brief enables or disables MXU
 * @details and checks if it applied correctly
 * @param enable enable MXU when true, disable otherwise
 * @param quiet don't print any errors
 * @retval true operation done successful, even if it already was done
 * @retval false can't change state properly, without changing state of other flags in MXU_CR
 */
bool mxu_enable(bool enable, bool quiet);

uint32_t random32(void);
uint64_t random64(void);

#define PRINT_TEST_RESULT(insn,cond) \
	printf("TEST %16s %s\n", #insn, ((cond) ? "[PASS]" : "[FAIL]"))

