#pragma once
#include <threads.h>

#include "vec.h"

typedef struct {
    vec3 *array;
    usize begin;
    usize end;
    vec3 (*func)(usize);
} pfor_vec3_t;

int loop_vec3_for(void *arg);
void pfor_vec3(vec3 *array, usize N, usize num_threads, vec3 (*func)(usize));
