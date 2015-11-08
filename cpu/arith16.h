/*
 * arith16.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef ARITH16_H_
#define ARITH16_H_

#include "../common.h"
#include "../gem.h"
#include "../cpu.h"
#include "../mmu.h"

#define DECLARE_INC_R16(REG16) void INC_ ## REG16 (struct machine_t *)
#define DECLARE_DEC_R16(REG16) void DEC_ ## REG16 (struct machine_t *)
#define DECLARE_ADD_HL_R16(REG16) void ADD_HL_ ## REG16 (struct machine_t *)


DECLARE_OP_REGS16(INC)
DECLARE_OP_REGS16(DEC)
DECLARE_OP_REGS16(ADD_HL)

void ADD_SP_r8(struct machine_t *);

#endif /* ARITH16_H_ */
