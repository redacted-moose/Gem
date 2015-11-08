/*
 * misc.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef MISC_H_
#define MISC_H_

#include "../common.h"

void NOP(struct machine_t *gem);
void STOP(struct machine_t *gem);
void HALT(struct machine_t *gem);
void DI(struct machine_t *gem);
void EI(struct machine_t *gem);
void XX(struct machine_t *gem);

#endif /* MISC_H_ */
