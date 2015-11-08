#include "../common.h"
#include "../gem.h"
#include "../cpu/arith8.h"
#include "../cpu.h"
#include "../mmu.h"
#include "../gpu.h"
#include "../input.h"
#include "unity.h"

struct machine_t gem;

void setUp(void) {
    gem.cpu = reset_cpu();
    gem.mmu = reset_mmu();
    gem.gpu = reset_gpu();
    gem.key = reset_key();
}

void tearDown(void) {
    destroy_cpu(gem.cpu);
    destroy_mmu(gem.mmu);
    destroy_gpu(gem.gpu);
    destroy_key(gem.key);
}

void test_INC_B(void) {
    INC_B(&gem);
    TEST_ASSERT_EQUAL_HEX8(1, gem.cpu->b);
    TEST_ASSERT_FALSE(gem.cpu->z);
    TEST_ASSERT_FALSE(gem.cpu->hc);
    TEST_ASSERT_FALSE(gem.cpu->n);
}

void test_INC_B_overflow(void) {
    gem.cpu->b = 0xFF;
    INC_B(&gem);
    TEST_ASSERT_EQUAL_HEX8(0, gem.cpu->b);
    TEST_ASSERT_TRUE(gem.cpu->z);
    TEST_ASSERT_TRUE(gem.cpu->hc);
    TEST_ASSERT_FALSE(gem.cpu->n);
}

void test_INC_B_hc(void) {
    gem.cpu->b = 0x7F;
    INC_B(&gem);
    TEST_ASSERT_EQUAL_HEX8(0x80, gem.cpu->b);
    TEST_ASSERT_FALSE(gem.cpu->z);
    TEST_ASSERT_TRUE(gem.cpu->hc);
    TEST_ASSERT_FALSE(gem.cpu->n);
}

void test_INC_C_overflow(void) {
    byte temp = gem.cpu->b;
    gem.cpu->c = 0xFF;
    INC_C(&gem);
    TEST_ASSERT_EQUAL_HEX8(0, gem.cpu->c);
    TEST_ASSERT_TRUE(gem.cpu->z);
    TEST_ASSERT_TRUE(gem.cpu->hc);
    TEST_ASSERT_FALSE(gem.cpu->n);
    TEST_ASSERT_EQUAL_HEX8(temp, gem.cpu->b);
}

void test_DEC_B(void) {
    gem.cpu->b = 2;
    DEC_B(&gem);
    TEST_ASSERT_EQUAL_HEX8(1, gem.cpu->b);
    TEST_ASSERT_FALSE(gem.cpu->z);
    TEST_ASSERT_FALSE(gem.cpu->hc);
    TEST_ASSERT_FALSE(gem.cpu->n);
}

void test_DEC_B_overflow(void) {
    gem.cpu->b = 0;
    DEC_B(&gem);
    TEST_ASSERT_EQUAL_HEX8(0xFF, gem.cpu->b);
    TEST_ASSERT_TRUE(gem.cpu->z);
    TEST_ASSERT_TRUE(gem.cpu->hc);
    TEST_ASSERT_FALSE(gem.cpu->n);
}

void test_DEC_B_hc(void) {
    gem.cpu->b = 0x80;
    DEC_B(&gem);
    TEST_ASSERT_EQUAL_HEX8(0x7F, gem.cpu->b);
    TEST_ASSERT_FALSE(gem.cpu->z);
    TEST_ASSERT_TRUE(gem.cpu->hc);
    TEST_ASSERT_FALSE(gem.cpu->n);
}

void test_DEC_C_overflow(void) {
    byte temp = gem.cpu->b;
    gem.cpu->c = 0;
    DEC_C(&gem);
    TEST_ASSERT_EQUAL_HEX8(0xFF, gem.cpu->c);
    TEST_ASSERT_TRUE(gem.cpu->z);
    TEST_ASSERT_TRUE(gem.cpu->hc);
    TEST_ASSERT_FALSE(gem.cpu->n);
    TEST_ASSERT_EQUAL_HEX8(temp, gem.cpu->b);
}

void test_CPL(void) {
    byte save = gem.cpu->a;
    CPL(&gem);
    TEST_ASSERT_EQUAL_HEX8(~save, gem.cpu->a);
    TEST_ASSERT_TRUE(gem.cpu->n);
    TEST_ASSERT_TRUE(gem.cpu->hc);
}

void test_SCF(void) {
    SCF(&gem);
    TEST_ASSERT_TRUE(gem.cpu->ca);
    TEST_ASSERT_FALSE(gem.cpu->n);
    TEST_ASSERT_FALSE(gem.cpu->hc);
}

void test_CCF(void) {
    CCF(&gem);
    TEST_ASSERT_FALSE(gem.cpu->n);
    TEST_ASSERT_FALSE(gem.cpu->hc);

}
