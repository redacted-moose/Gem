/*
 * bit8.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef BIT8_H_
#define BIT8_H_

#include "../common.h"
#include "../gem.h"

#define DECLARE_OP_REGS(OP)                     \
    void OP ## _B (struct machine_t *gem);      \
    void OP ## _C (struct machine_t *gem);      \
    void OP ## _D (struct machine_t *gem);      \
    void OP ## _E (struct machine_t *gem);      \
    void OP ## _H (struct machine_t *gem);      \
    void OP ## _L (struct machine_t *gem);      \
    void OP ## _IHL (struct machine_t *gem);    \
    void OP ## _A (struct machine_t *gem);

void RLCA(struct machine_t *gem);
void RRCA(struct machine_t *gem);
void RLA(struct machine_t *gem);
void RRA(struct machine_t *gem);
void PREFIX_CB(struct machine_t *gem);

DECLARE_OP_REGS(RLC)
DECLARE_OP_REGS(RRC)
DECLARE_OP_REGS(RL)
DECLARE_OP_REGS(RR)
DECLARE_OP_REGS(SLA)
DECLARE_OP_REGS(SRA)
DECLARE_OP_REGS(SWAP)
DECLARE_OP_REGS(SRL)

DECLARE_OP_REGS(BIT_0)
DECLARE_OP_REGS(BIT_1)
DECLARE_OP_REGS(BIT_2)
DECLARE_OP_REGS(BIT_3)
DECLARE_OP_REGS(BIT_4)
DECLARE_OP_REGS(BIT_5)
DECLARE_OP_REGS(BIT_6)
DECLARE_OP_REGS(BIT_7)

DECLARE_OP_REGS(RES_0)
DECLARE_OP_REGS(RES_1)
DECLARE_OP_REGS(RES_2)
DECLARE_OP_REGS(RES_3)
DECLARE_OP_REGS(RES_4)
DECLARE_OP_REGS(RES_5)
DECLARE_OP_REGS(RES_6)
DECLARE_OP_REGS(RES_7)

DECLARE_OP_REGS(SET_0)
DECLARE_OP_REGS(SET_1)
DECLARE_OP_REGS(SET_2)
DECLARE_OP_REGS(SET_3)
DECLARE_OP_REGS(SET_4)
DECLARE_OP_REGS(SET_5)
DECLARE_OP_REGS(SET_6)
DECLARE_OP_REGS(SET_7)

#endif /* BIT8_H_ */
