#include "cpu_load.h"
#include <inttypes.h>
#include <stdio.h>
bool cpu_snapshot_read(cpu_snapshot *output)
{
    if (!output) {
        return false;
    }
    FILE *file = fopen("/proc/stat", "r");
    if (!file) {
        return false;
    }
    cpu_snapshot snapshot;
    int count = fscanf(file, "cpu %" SCNu64 " %" SCNu64 " %" SCNu64
        " %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64 " %" SCNu64,
        &snapshot.fields[0], &snapshot.fields[1], &snapshot.fields[2],
        &snapshot.fields[3], &snapshot.fields[4], &snapshot.fields[5],
        &snapshot.fields[6], &snapshot.fields[7]);
    bool ok = count == 8;
    if (fclose(file) != 0) {
        ok = false;
    }
    if (ok) {
        *output = snapshot;
    }
    return ok;
}
bool cpu_load_percent(const cpu_snapshot *before, const cpu_snapshot *after, double *output)
{
    if (!before || !after || !output) {
        return false;
    }
    double total = 0.0;
    double inactive = 0.0;
    for (size_t i = 0; i < 8; ++i) {
        if (after->fields[i] < before->fields[i]) {
            return false;
        }
        double delta = (double)(after->fields[i] - before->fields[i]);
        total += delta;
        if (i == 3 || i == 4 || i == 7) {
            inactive += delta;
        }
    }
    if (total == 0.0) {
        return false;
    }
    *output = 100.0 * (total - inactive) / total;
    return true;
}
