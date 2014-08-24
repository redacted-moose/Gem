/*
 * control.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef CONTROL_H_
#define CONTROL_H_

void JR_r8();
void JR_NZ_r8();
void JR_Z_r8();
void JR_NC_r8();
void JR_C_r8();
void RET_NZ();
void JP_NZ_a16();
void JP_a16();
void CALL_NZ_a16();
void RST_00H();
void RET_Z();
void RET();
void JP_Z_a16();
void CALL_Z_a16();
void CALL_a16();
void RST_08H();
void RET_NC();
void JP_NC_a16();
void CALL_NC_a16();
void RST_10H();
void RET_C();
void RETI();
void JP_C_a16();
void CALL_C_a16();
void RST_18H();
void RST_20H();
void JP_HL();
void RST_28H();
void RST_30H();
void RST_38H();

#endif /* CONTROL_H_ */
