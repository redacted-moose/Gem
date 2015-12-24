#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef uint8_t   byte;
typedef uint16_t  word;
typedef int8_t    s_byte; // Signed byte
typedef int16_t   s_word; // Signed word

#define PRINT(...)  fprintf(stderr, __VA_ARGS__)
#define ERROR(M, ...)  PRINT("[ERROR]\t(%s:%d)\t" M, __FILE__, __LINE__, ##__VA_ARGS__)

#if defined (DEBUG)
#define INFO(M, ...)	PRINT("[DEBUG]\t(%s:%d)\t" M, __FILE__, __LINE__, ##__VA_ARGS__)
#define WARN(M, ...)	PRINT("[ WARN]\t(%s:%d)\t" M, __FILE__, __LINE__, ##__VA_ARGS__)
#elif defined (WARN)
#define INFO(M, ...)   ((void)0)
#define WARN(M, ...)   PRINT("[ WARN]\t(%s:%d)\t" M, __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define INFO(M, ...)   ((void)0)
#define WARN(M, ...)   ((void)0)
#endif

struct machine_t {
    struct cpu_t *cpu;
    struct gpu_t *gpu;
    struct mmu_t *mmu;
    struct key_t *key;

    bool run;
};

#ifdef DEBUG
extern struct machine_t *gem_debug;
#endif

#define DECLARE_OP_REGS(OP)                     \
    void OP ## _B (struct machine_t *gem);      \
    void OP ## _C (struct machine_t *gem);      \
    void OP ## _D (struct machine_t *gem);      \
    void OP ## _E (struct machine_t *gem);      \
    void OP ## _H (struct machine_t *gem);      \
    void OP ## _L (struct machine_t *gem);      \
    void OP ## _IHL (struct machine_t *gem);    \
    void OP ## _A (struct machine_t *gem);

#define DECLARE_OP_REGS16(OP)                   \
    void OP ## _BC (struct machine_t *gem);     \
    void OP ## _DE (struct machine_t *gem);     \
    void OP ## _HL (struct machine_t *gem);     \
    void OP ## _SP (struct machine_t *gem);

#define DECLARE_OP_REGS16_AF(OP)\
    void OP ## _BC (struct machine_t *gem);\
    void OP ## _DE (struct machine_t *gem);\
    void OP ## _HL (struct machine_t *gem);\
    void OP ## _AF (struct machine_t *gem);

#define GET_MACRO(_1,_2,_3,NAME,...) NAME

struct machine_t *init();
void destroy(struct machine_t *);
void execute(struct machine_t *);

#endif /* COMMON_H_ */
