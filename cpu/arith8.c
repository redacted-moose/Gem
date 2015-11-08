/*
 * arith8.c
 *
 *  Created on: Aug 16, 2014
 *      Author: sadlercr
 */

#include "../cpu.h"
#include "../mmu.h"
#include "arith8.h"


#define INC_R8(REG8, reg8)                          \
    void INC_ ## REG8 (struct machine_t *gem) {     \
        struct cpu_t *cpu = gem->cpu;               \
        cpu->reg8++;                                \
        cpu->hc = (cpu->reg8 & 0x0F) ? 0 : 1;       \
        cpu->z = (cpu->reg8) ? 0 : 1;               \
        cpu->n = 0;                                 \
    }

INC_R8(B, b)
INC_R8(C, c)
INC_R8(D, d)
INC_R8(E, e)
INC_R8(H, h)
INC_R8(L, l)
INC_R8(IHL, ihl)
INC_R8(A, a)


#define DEC_R8(REG8, reg8)                          \
    void DEC_ ## REG8 (struct machine_t *gem) {     \
        struct cpu_t *cpu = gem->cpu;               \
        cpu->reg8--;                                \
        cpu->hc = (~(cpu->reg8 & 0x0F)) ? 1 : 0;    \
        cpu->z = (cpu->reg8) ? 0 : 1;               \
        cpu->n = 1;                                 \
    }

DEC_R8(B, b)
DEC_R8(C, c)
DEC_R8(D, d)
DEC_R8(E, e)
DEC_R8(H, h)
DEC_R8(L, l)
DEC_R8(IHL, ihl)
DEC_R8(A, a)

void DAA(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;

	// TODO: implement DAA
}


void CPL(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->a = ~cpu->a;
	cpu->n = 1;
	cpu->hc = 1;
}


void SCF(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->ca = 1;
	cpu->n = 0;
	cpu->hc = 0;
}


void CCF(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->n = 0;
	cpu->hc = 0;
	cpu->ca = ~cpu->ca;
}


#define ADD_A_R8(REG8, reg8)                        \
    void ADD_A_ ## REG8 (struct machine_t *gem) {   \
        struct cpu_t *cpu = gem->cpu;               \
        byte bef_byte = cpu->a;                     \
        byte bef = cpu->a & 0x0F;                   \
        cpu->a += cpu->reg8;                        \
        cpu->hc = ((cpu->a & 0x0F) < bef) ? 1 : 0;  \
        cpu->ca = (cpu->a < bef_byte) ? 1 : 0;      \
        cpu->n = 0;                                 \
        cpu->z = (cpu->a) ? 0 : 1;                  \
    }

ADD_A_R8(B, b)
ADD_A_R8(C, c)
ADD_A_R8(D, d)
ADD_A_R8(E, e)
ADD_A_R8(H, h)
ADD_A_R8(L, l)
ADD_A_R8(IHL, ihl)
ADD_A_R8(A, a)


#define ADC_A_R8(REG8, reg8)                        \
    void ADC_A_ ## REG8 (struct machine_t *gem) {   \
        struct cpu_t *cpu = gem->cpu;               \
        byte bef_byte = cpu->a;                     \
        byte bef = cpu->a & 0x0F;                   \
        cpu->a += cpu->reg8 + cpu->ca;              \
        cpu->hc = ((cpu->a & 0x0F) < bef) ? 1 : 0;  \
        cpu->ca = (cpu->a < bef_byte) ? 1 : 0;      \
        cpu->n = 0;                                 \
        cpu->z = (cpu->a) ? 0 : 1;                  \
    }

ADC_A_R8(B, b)
ADC_A_R8(C, c)
ADC_A_R8(D, d)
ADC_A_R8(E, e)
ADC_A_R8(H, h)
ADC_A_R8(L, l)
ADC_A_R8(IHL, ihl)
ADC_A_R8(A, a)


#define SUB_R8(REG8, reg8)                      \
    void SUB_ ## REG8 (struct machine_t *gem) {     \
        struct cpu_t *cpu = gem->cpu;               \
        byte bef_byte = cpu->a;                     \
        byte bef = cpu->a & 0x0F;                   \
        cpu->a -= cpu->reg8;                        \
        cpu->hc = ((cpu->a & 0x0F) < bef) ? 1 : 0;  \
        cpu->ca = (cpu->a < bef_byte) ? 1 : 0;      \
        cpu->n = 1;                                 \
        cpu->z = (cpu->a) ? 0 : 1;                  \
    }

SUB_R8(B, b)
SUB_R8(C, c)
SUB_R8(D, d)
SUB_R8(E, e)
SUB_R8(H, h)
SUB_R8(L, l)
SUB_R8(IHL, ihl)
SUB_R8(A, a)


#define SBC_A_R8(REG8, reg8)                        \
    void SBC_A_ ## REG8 (struct machine_t *gem) {   \
        struct cpu_t *cpu = gem->cpu;               \
        byte bef_byte = cpu->a;                     \
        byte bef = cpu->a & 0x0F;                   \
        cpu->a -= cpu->reg8 + cpu->ca;              \
        cpu->hc = ((cpu->a & 0x0F) < bef) ? 1 : 0;  \
        cpu->ca = (cpu->a < bef_byte) ? 1 : 0;      \
        cpu->n = 1;                                 \
        cpu->z = (cpu->a) ? 0 : 1;                  \
    }

SBC_A_R8(B, b)
SBC_A_R8(C, c)
SBC_A_R8(D, d)
SBC_A_R8(E, e)
SBC_A_R8(H, h)
SBC_A_R8(L, l)
SBC_A_R8(IHL, ihl)
SBC_A_R8(A, a)


#define AND_R8(REG8, reg8)                      \
    void AND_ ## REG8 (struct machine_t *gem) { \
        struct cpu_t *cpu = gem->cpu;           \
        cpu->a &= cpu->reg8;                    \
        cpu->z = (cpu->a) ? 0 : 1;              \
        cpu->n = 0;                             \
        cpu->h = 1;                             \
        cpu->ca = 0;                            \
    }

AND_R8(B, b)
AND_R8(C, c)
AND_R8(D, d)
AND_R8(E, e)
AND_R8(H, h)
AND_R8(L, l)
AND_R8(IHL, ihl)
AND_R8(A, a)


#define XOR_R8(REG8, reg8)                      \
    void XOR_ ## REG8 (struct machine_t *gem) { \
        struct cpu_t *cpu = gem->cpu;           \
        cpu->a ^= cpu->reg8;                    \
        cpu->z = (cpu->a) ? 0 : 1;              \
        cpu->n = 0;                             \
        cpu->h = 0;                             \
        cpu->ca = 0;                            \
    }

XOR_R8(B, b)
XOR_R8(C, c)
XOR_R8(D, d)
XOR_R8(E, e)
XOR_R8(H, h)
XOR_R8(L, l)
XOR_R8(IHL, ihl)
XOR_R8(A, a)


#define OR_R8(REG8, reg8)                       \
    void OR_ ## REG8 (struct machine_t *gem) {  \
        struct cpu_t *cpu = gem->cpu;           \
        cpu->a |= cpu->reg8;                    \
        cpu->z = (cpu->a) ? 0 : 1;              \
        cpu->n = 0;                             \
        cpu->h = 0;                             \
        cpu->ca = 0;                            \
    }

OR_R8(B, b)
OR_R8(C, c)
OR_R8(D, d)
OR_R8(E, e)
OR_R8(H, h)
OR_R8(L, l)
OR_R8(IHL, ihl)
OR_R8(A, a)


#define CP_R8(REG8, reg8)                                       \
    void CP_ ## REG8 (struct machine_t *gem) {                  \
        struct cpu_t *cpu = gem->cpu;                           \
        byte after = cpu->a - cpu->reg8;                        \
        cpu->hc = ((cpu->a & 0x0F) < (after & 0x0F)) ? 1 : 0;   \
        cpu->ca = (cpu->a < after) ? 1 : 0;                     \
        cpu->n = 1;                                             \
        cpu->z = (after) ? 0 : 1;                               \
    }

CP_R8(B, b)
CP_R8(C, c)
CP_R8(D, d)
CP_R8(E, e)
CP_R8(H, h)
CP_R8(L, l)
CP_R8(IHL, ihl)
CP_R8(A, a)

void ADD_A_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	byte bef_byte = cpu->a;
	byte bef = cpu->a & 0x0F;
	cpu->a += read_byte(gem, cpu->pc);
	cpu->pc++;
	cpu->hc = ((cpu->a & 0x0F) < bef) ? 1 : 0;
	cpu->ca = (cpu->a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu->n = 0;
	cpu->z = (cpu->a) ? 0 : 1;
}

void ADC_A_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	byte bef_byte = cpu->a;
	byte bef = cpu->a & 0x0F;
	cpu->a += read_byte(gem, cpu->pc) + cpu->ca;
	cpu->pc++;
	cpu->hc = ((cpu->a & 0x0F) < bef) ? 1 : 0;
	cpu->ca = (cpu->a < bef_byte) ? 1 : 0; // Overflow occurred, so set carry bit
	cpu->n = 0;
	cpu->z = (cpu->a) ? 0 : 1;
}

void SUB_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	byte bef_byte = cpu->a;
	byte bef = cpu->a & 0x0F;
	cpu->a -= read_byte(gem, cpu->pc);
	cpu->pc++;
	cpu->hc = ((cpu->a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu->ca = (cpu->a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu->n = 1;
	cpu->z = (cpu->a) ? 0 : 1;
}

void SBC_A_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	byte bef_byte = cpu->a;
	byte bef = cpu->a & 0x0F;
	cpu->a -= read_byte(gem, cpu->pc) + cpu->ca;
	cpu->pc++;
	cpu->hc = ((cpu->a & 0x0F) < bef) ? 1 : 0; // Set if no half-borrow
	cpu->ca = (cpu->a < bef_byte) ? 1 : 0; // Set if no borrow
	cpu->n = 1;
	cpu->z = (cpu->a) ? 0 : 1;
}

void AND_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->a &= read_byte(gem, cpu->pc);
	cpu->pc++;
	cpu->z = (cpu->a) ? 0 : 1;
	cpu->n = 0;
	cpu->h = 1;
	cpu->ca = 0;
}

void XOR_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->a ^= read_byte(gem, cpu->pc);
	cpu->pc++;
	cpu->z = (cpu->a) ? 0 : 1;
	cpu->n = 0;
	cpu->h = 0;
	cpu->ca = 0;
}

void OR_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	cpu->a |= read_byte(gem, cpu->pc);
	cpu->pc++;
	cpu->z = (cpu->a) ? 0 : 1;
	cpu->n = 0;
	cpu->h = 0;
	cpu->ca = 0;
}

void CP_d8(struct machine_t *gem) {
    struct cpu_t *cpu = gem->cpu;
	byte after = cpu->a - read_byte(gem, cpu->pc);
	cpu->pc++;
	cpu->hc = ((cpu->a & 0x0F) < (after & 0x0F)) ? 1 : 0; // Set if no half-borrow
	cpu->ca = (cpu->a < after) ? 1 : 0; // Set if no borrow
	cpu->n = 1;
	cpu->z = (after) ? 0 : 1;
}
