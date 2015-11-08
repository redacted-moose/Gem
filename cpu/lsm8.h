/*
 * lsm8.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef LSM8_H_
#define LSM8_H_

#include "../common.h"

void LD_BC_A();
void LD_B_d8();
void LD_A_BC();
void LD_C_d8();
void LD_DE_A();
void LD_D_d8();
void LD_A_DE();
void LD_E_d8();
void LDI_HL_A();
void LD_H_d8();
void LDI_A_HL();
void LD_L_d8();
void LDD_HL_A();
void LD_IHL_d8();
void LDD_A_HL();
void LD_A_d8();

DECLARE_OP_REGS(LD_B)
DECLARE_OP_REGS(LD_C)
DECLARE_OP_REGS(LD_D)
DECLARE_OP_REGS(LD_E)
DECLARE_OP_REGS(LD_H)
DECLARE_OP_REGS(LD_L)

void LD_IHL_B();
void LD_IHL_C();
void LD_IHL_D();
void LD_IHL_E();
void LD_IHL_H();
void LD_IHL_L();
void LD_IHL_A();
/* DECLARE_OP_REGS(LD_IHL) */

DECLARE_OP_REGS(LD_A)

void LDH_a8_A();
void LD_IC_A();
void LD_a16_A();
void LDH_A_a8();
void LD_A_IC();
void LD_A_a16();

#endif /* LSM8_H_ */
