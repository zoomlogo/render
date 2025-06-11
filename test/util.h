// unit testing utilities
#pragma once
#include <stdio.h>
#include "../src/types.h"

#define INIT_TEST() extern usize test_num, passed
#define ASSERT(cond) \
    ({ test_num++; \
    if ( cond ) passed++; \
    else { \
        printf("test failed @ %s:%d\n", __FILE__, __LINE__); \
    } })
#define BEGIN_TEST() printf("entering %s...\n", __FILE__); test_num = 0, passed = 0
#define END_TEST() printf("%lu / %lu passed.\n", passed, test_num)
