#define _GNU_SOURCE
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

double tdiff_timeval(struct timeval *start, struct timeval *end) {
  return (end->tv_sec-start->tv_sec) + 1e-6*(end->tv_usec-start->tv_usec);
}
double tdiff_timespec(struct timespec *start, struct timespec *end) {
  return (end->tv_sec-start->tv_sec) + 1e-9*(end->tv_nsec-start->tv_nsec);
}

#define N 256
double x[N];

void init(void) {
    srandom(0);
    for (size_t j = 0; j < N; j++) {
        x[j] = random();
    }
}


double loop(void) {
    for (size_t t = 0; t < N; t++) {
        for (size_t j = 0; j < N; j++) {
            x[j] += sin(x[j]);
        }
    }
    double sum = 0;
    for (size_t j = 0; j < N; j++) {
        sum += x[j];
    }
    return sum;
}


int n_trials = 5;

void timeit_gettimeofday(void) {
    struct timeval start, end;
    printf("gettimeofday:\n");
    double sum_squares = 0;
    double sum         = 0;
    double min = 0, max = 0;
    for (int trial = 0; trial < n_trials; trial++) {
        init();
        gettimeofday(&start, NULL);
        loop();
        gettimeofday(&end,   NULL);
        double tim = tdiff_timeval(&start, &end);
        sum_squares += tim*tim;
        sum         += tim;
        if (trial==0) min = max = tim;
        else {
            if (tim < min) min = tim;
            if (tim > max) max = tim;
        }

        printf(" %12.9fs\n", tim);
    }
    double avg_x = sum/n_trials;
    double avg_xsquared = sum_squares/n_trials;
    printf("  mean=%12.9fs sqrt(variance)=%12.9fs relative_range=%5.2f\n",
           avg_x, sqrt(avg_xsquared-avg_x), (max-min)/max);
}

void timeit_clock_gettime(clockid_t clk_id, const char *string) {
    struct timespec start, end;
    printf("%s\n", string);
    double sum_squares = 0;
    double sum         = 0;
    double min = 0, max = 0;
    for (int trial = 0; trial < n_trials; trial++) {
        init();
        clock_gettime(clk_id, &start);
        loop();
        clock_gettime(clk_id, &end);
        double tim = tdiff_timespec(&start, &end);
        sum_squares += tim*tim;
        sum         += tim;
        if (trial==0) min = max = tim;
        else {
            if (tim < min) min = tim;
            if (tim > max) max = tim;
        }
        printf(" %12.9fs\n", tim);
    }
    double avg_x = sum/n_trials;
    double avg_xsquared = sum_squares/n_trials;
    printf("  mean=%12.9fs sqrt(variance)=%12.9fs relative_range=%5.2f\n",
           avg_x, sqrt(avg_xsquared-avg_x), (max-min)/max);
}

#define CGT(n) timeit_clock_gettime(CLOCK_ ## n, #n)

int main(int argc __attribute__((unused)), const char *argv[] __attribute__((unused))) {
    timeit_gettimeofday();
    CGT(REALTIME);
    CGT(MONOTONIC);
    CGT(REALTIME_COARSE);
    CGT(MONOTONIC);
    CGT(MONOTONIC_COARSE);
    CGT(MONOTONIC_RAW);
    CGT(BOOTTIME);
    CGT(PROCESS_CPUTIME_ID);
    CGT(THREAD_CPUTIME_ID);
    return 0;
}
