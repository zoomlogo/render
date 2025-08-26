#include <threads.h>

#include "vec.h"
#include "pfor.h"

int loop_vec3_for(void *arg) {
    pfor_vec3_t *data = (pfor_vec3_t *) arg;
    for (usize i = data->begin; i < data->end; i++)
        data->array[i] = data->func(i);
    return 0;
}

void pfor_vec3(vec3 *array, usize N, usize num_threads, vec3 (*func)(usize)) {
    thrd_t threads[num_threads];
    pfor_vec3_t tdata[num_threads];
    usize chunk_size = N / num_threads;

    for (usize i = 0; i < num_threads; i++) {
        tdata[i].array = array;
        tdata[i].begin = i * chunk_size;
        tdata[i].end = (i == num_threads - 1) ? N : (i + 1) * chunk_size;
        tdata[i].func = func;
        thrd_create(&threads[i], loop_vec3_for, &tdata[i]);
    }

    for (usize i = 0; i < num_threads; i++)
        thrd_join(threads[i], NULL);
}
