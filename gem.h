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
#include <stdbool.h>

typedef uint8_t byte;
typedef uint16_t word;
typedef int8_t s_byte; // Signed byte
typedef int16_t s_word; // Signed word

#define PRINT(...)  fprintf(stderr, __VA_ARGS__)
#define ERROR(...)  PRINT("ERROR: " __VA_ARGS__)

#if defined (DEBUG)
#define INFO(...)	PRINT("DEBUG: " __VA_ARGS__)
#define WARN(...)	PRINT("WARNING: " __VA_ARGS__)
#elif defined (WARN)
#define INFO(...)   ((void)0)
#define WARN(...)   PRINT("WARNING: " __VA_ARGS__)
#else
#define INFO(...)   ((void)0)
#define WARN(...)   ((void)0)
#endif

extern bool run;

#endif /* GEM_H_ */
