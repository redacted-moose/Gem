/*
 * lsm16.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef LSM16_H_
#define LSM16_H_

#include "../common.h"

void LD_BC_d16(struct machine_t *gem);
void LD_a16_SP(struct machine_t *gem);
void LD_DE_d16(struct machine_t *gem);
void LD_HL_d16(struct machine_t *gem);
void LD_SP_d16(struct machine_t *gem);
DECLARE_OP_REGS16_AF(POP)
DECLARE_OP_REGS16_AF(PUSH)
void LD_HL_SP_r8(struct machine_t *gem);
void LD_SP_HL(struct machine_t *gem);

#endif /* LSM16_H_ */
