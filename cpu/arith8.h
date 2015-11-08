/*
 * arith8.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef ARITH8_H_
#define ARITH8_H_

#include "../common.h"
#include "../gem.h"
#include "../cpu.h"

#define DECLARE_INC_R8(REG8) void INC_ ## REG8 (struct machine_t *)
#define DECLARE_DEC_R8(REG8) void DEC_ ## REG8 (struct machine_t *)
#define DECLARE_ADD_A_R8(REG8) void ADD_A_ ## REG8 (struct machine_t *)
#define DECLARE_ADC_A_R8(REG8) void ADC_A_ ## REG8 (struct machine_t *)
#define DECLARE_SUB_R8(REG8) void SUB_ ## REG8 (struct machine_t *)
#define DECLARE_SBC_A_R8(REG8) void SBC_A_ ## REG8 (struct machine_t *)
#define DECLARE_AND_R8(REG8) void AND_ ## REG8 (struct machine_t *)
#define DECLARE_XOR_R8(REG8) void XOR_ ## REG8 (struct machine_t *)
#define DECLARE_OR_R8(REG8) void OR_ ## REG8 (struct machine_t *)
#define DECLARE_CP_R8(REG8) void CP_ ## REG8 (struct machine_t *)

void DAA(struct machine_t *);
void CPL(struct machine_t *);
void SCF(struct machine_t *);
void CCF(struct machine_t *);
void ADD_A_d8(struct machine_t *);
void ADC_A_d8(struct machine_t *);
void SUB_d8(struct machine_t *);
void SBC_A_d8(struct machine_t *);
void AND_d8(struct machine_t *);
void XOR_d8(struct machine_t *);
void OR_d8(struct machine_t *);
void CP_d8(struct machine_t *);

DECLARE_OP_REGS(INC)
DECLARE_OP_REGS(DEC)
DECLARE_OP_REGS(ADD_A)
DECLARE_OP_REGS(ADC_A)
DECLARE_OP_REGS(SUB)
DECLARE_OP_REGS(SBC_A)
DECLARE_OP_REGS(AND)
DECLARE_OP_REGS(XOR)
DECLARE_OP_REGS(OR)
DECLARE_OP_REGS(CP)

#endif /* ARITH8_H_ */
