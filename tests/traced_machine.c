// Test-only translation unit. Release cores compile src/machine.c directly.
#include <stdio.h>
#include <stdint.h>
extern FILE *psg_trace;
static uint64_t cycle;
#define GAMATE_TRACE_CLOCK() (++cycle)
#define GAMATE_TRACE_PSG(reg, val)                                                                 \
    do {                                                                                           \
        if (psg_trace)                                                                             \
            fprintf(psg_trace, "%llu %u %u\n", (unsigned long long)(cycle - 1), (unsigned)(reg),   \
                    (unsigned)(val));                                                              \
    } while (0)
#include "../src/machine.c"
