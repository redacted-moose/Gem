/*
 * gem.h
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#ifndef GEM_H_
#define GEM_H_

#include <stdint.h>
#include <stdio.h>
//#include "cpu.h"
//#include "mmu.h"
//#include "gpu.h"

typedef uint8_t byte;
typedef uint16_t word;
typedef int8_t s_byte; // Signed byte
typedef int16_t s_word; // Signed word

#ifdef DEBUG
//	#define INFO(s)		printf("I: %s\n", s)
//	#define WARN(s)		printf("W: %s\n", s)
//	#define ERROR(s)	printf("E: %s\n", s)
//	#define INFO(...)	printf("I: "); printf(__VA_ARGS__)
//	#define WARN(...)	printf("W: "); printf(__VA_ARGS__)
//	#define ERROR(...)	printf("E: "); printf(__VA_ARGS__)

	#define INFO(...)	printf("I: " __VA_ARGS__)
	#define WARN(...)	printf("W: " __VA_ARGS__)
	#define ERROR(...)	printf("E: " __VA_ARGS__)
#else
	#define INFO(...)
	#define WARN(...)
	#define ERROR(...)
#endif

//typedef struct state_t {
//	cpu *cpu;
//	mmu *mmu;
//	gpu *gpu;
//} state;

#endif /* GEM_H_ */
