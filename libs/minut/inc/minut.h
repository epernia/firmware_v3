#ifndef MINUT_H
#define MINUT_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MINUT(enable)       \
    do {                    \
        if (!enable) break; \
        test();             \
        exit(EXIT_SUCCESS); \
    } while (0)

#define TEST(func_name) \
    bool (func_name) (void)

#define ASSERT_EQ(golden, result)                                     \
    do {                                                              \
        if (!((golden) == (result))) {                                \
            printf("test %s ... FAILED: ", __func__);                 \
            printf("assertion failed: \""#golden" == "#result"\"\r\n"); \
            return false;                                             \
        }                                                             \
        printf("test %s ... ok\r\n", __func__);                         \
        return true;                                                  \
    } while (0)

#define ASSERT_EQ_CMP(cmp, golden, result)                            \
    do {                                                              \
        if (!((cmp)((golden), (result)))) {                                \
            printf("test %s ... FAILED: ", __func__);                 \
            printf("assertion failed: \""#golden" == "#result"\"\r\n"); \
            return false;                                             \
        }                                                             \
        printf("test %s ... ok\r\n", __func__);                         \
        return true;                                                  \
    } while (0)

#define MINUT_BEG                  \
    void test(void) {              \
        unsigned failed = 0;       \
        unsigned passed = 0;       \
        unsigned current_test = 0;

#define RUN(test_function)                     \
    do {                                       \
        current_test++;                        \
        (test_function) ? passed++ : failed++; \
    } while (0)                                \

#define MINUT_END                                               \
        printf("test result: %s. ", failed ? "FAILED" : "ok");  \
        printf("%u passed; %u failed\r\n", passed, failed);       \
    }

void test(void);

#endif /* MINUT */

