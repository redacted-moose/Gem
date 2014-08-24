/*
 * lsm16.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef LSM16_H_
#define LSM16_H_

void LD_BC_d16();
void LD_a16_SP();
void LD_DE_d16();
void LD_HL_d16();
void LD_SP_d16();
void POP_BC();
void PUSH_BC();
void POP_DE();
void PUSH_DE();
void POP_HL();
void PUSH_HL();
void POP_AF();
void PUSH_AF();
void LD_HL_SP_r8();
void LD_SP_HL();

#endif /* LSM16_H_ */
