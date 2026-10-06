#ifndef CPU_LOAD_H
#define CPU_LOAD_H
#include <stdbool.h>
#include <stdint.h>
typedef struct cpu_snapshot {
    uint64_t fields[8];
} cpu_snapshot;
bool cpu_snapshot_read(cpu_snapshot *output);
/* False for reset/decreasing counters or an interval with no ticks. */
bool cpu_load_percent(const cpu_snapshot *before, const cpu_snapshot *after, double *output);
#endif
