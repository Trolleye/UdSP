#include "cpu_load.h"
#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <seconds>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int seconds = atoi(argv[1]);

    linked_list samples;
    linked_list_init(&samples);

    cpu_snapshot previous;
    if (!cpu_snapshot_read(&previous)) {
        fprintf(stderr, "Error reading /proc/stat\n");
        return EXIT_FAILURE;
    }

    for (int i = 1; i <= seconds; ++i) {
        sleep(1);

        cpu_snapshot current;
        if (!cpu_snapshot_read(&current)) {
            fprintf(stderr, "Error reading /proc/stat\n");
            break;
        }

        double load;
        if (cpu_load_percent(&previous, &current, &load)) {
            linked_list_add(&samples, load);
            printf("Second %d | CPU Load: %.2f%%\n", i, load);
        } else {
            printf("Second %d | Could not calculate load\n", i);
        }

        previous = current;
    }

    linked_list_cleanup(&samples);
    return EXIT_SUCCESS;
}



/*
#define _POSIX_C_SOURCE 200809L
#include "cpu_load.h"
#include "linked_list.h"
#include <errno.h>
#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static volatile sig_atomic_t stopped;
static void stop_handler(int signal_number)
{
    (void)signal_number;
    stopped = 1;
}
typedef struct average_result {
    double sum;
    size_t count;
} average_result;
static void accumulate(double data, void *output)
{
    average_result *result = output;
    result->sum += data;
    ++result->count;
}
static bool parse_seconds(const char *text, uintmax_t *seconds)
{
    if (!text || !*text) {
        return false;
    }
    for (const char *c = text; *c; ++c) {
        if (*c < '0' || *c > '9') {
            return false;
        }
    }
    errno = 0;
    char *end;
    uintmax_t value = strtoumax(text, &end, 10);
    //Conservative bound keeps tick count and deadlines manageable.
    if (errno || *end || value == 0 || value > 86400) {
        return false;
    }
    *seconds = value;
    return true;
}
int main(int argc, char **argv)
{
    uintmax_t seconds;
    if (argc != 2 || !parse_seconds(argv[1], &seconds)) {
        fprintf(stderr, "Pouzitie: %s SEKUNDY (cele cislo 1..86400)\n", argv[0]);
        return EXIT_FAILURE;
    }
    struct sigaction action = {0};
    action.sa_handler = stop_handler;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGINT, &action, NULL) || sigaction(SIGTERM, &action, NULL)) {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    linked_list samples;
    linked_list_init(&samples);
    int status = EXIT_FAILURE;
    cpu_snapshot previous;
    struct timespec deadline;
    if (!cpu_snapshot_read(&previous)) {
        fprintf(stderr, "Neda sa citat /proc/stat.\n");
        goto cleanup;
    }
    if (clock_gettime(CLOCK_MONOTONIC, &deadline)) {
        perror("clock_gettime");
        goto cleanup;
    }
    for (uintmax_t tick = 1; tick <= seconds * 10 && !stopped; ++tick) {
        deadline.tv_nsec += 100000000L;
        if (deadline.tv_nsec >= 1000000000L) {
            deadline.tv_nsec -= 1000000000L;
            ++deadline.tv_sec;
        }
        int error;
        do {
            error = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                                    &deadline, NULL);
        }
        while (error == EINTR && !stopped);
        if (stopped) {
            break;
        }
        if (error) {
            errno = error;
            perror("clock_nanosleep");
            goto cleanup;
        }
        cpu_snapshot current;
        if (!cpu_snapshot_read(&current)) {
            fprintf(stderr, "Chyba citania /proc/stat.\n");
            goto cleanup;
        }
        double load;
        if (cpu_load_percent(&previous, &current, &load)) {
            if (!linked_list_add(&samples, load)) {
                fprintf(stderr, "Nedostatok pamate.\n");
                goto cleanup;
            }
        } else {
            fprintf(stderr, "Vzorka %ju nema platny rozdiel pocitadiel; preskakujem.\n", tick);
        }
        previous = current;
        if (tick % 10 == 0) {
            average_result result = {0};
            linked_list_for_each(&samples, accumulate, &result);
            if (result.count) {
                printf("%ju s | vzorky: %zu | priemer CPU od zaciatku: %.2f %%\n",
                       tick / 10, result.count, result.sum / (double)result.count);
            } else {
                printf("%ju s | ziadne platne vzorky\n", tick / 10);
            }
            fflush(stdout);
        }
    }
    status = EXIT_SUCCESS;
cleanup:
    linked_list_cleanup(&samples);
    return status;
}
*/