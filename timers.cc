#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <vector>

double tdiff_timeval(struct timeval *start, struct timeval *end) {
  return (end->tv_sec-start->tv_sec) + 1e-6*(end->tv_usec-start->tv_usec);
}
double tdiff_timespec(struct timespec *start, struct timespec *end) {
  return (end->tv_sec-start->tv_sec) + 1e-9*(end->tv_nsec-start->tv_nsec);
}

class statistics {
    double min, max;
    double sum;
    double sum_squares;
    uint64_t n;
  public:
    statistics() : min(0), max(0), sum(0), sum_squares(0), n(0) {
    }
    void measurement(double f) {
        if (n == 0) {
            min = max = f;
        } else {
            min = std::min(min, f);
            max = std::max(max, f);
        }
        sum += f;
        sum_squares += f*f;
        n++;
    }
    double mean() {
        return sum/n;
    }
    double sigma() {
        double Ex2 = sum_squares/n;
        double Ex  = sum/n;
        return sqrt(Ex2 - Ex*Ex);
    }
    void print() {
        printf("  mean=%12.9fs sigma=%12.9fs (max-min)/mean=%7.4f%%\n",
               mean(), sigma(), 100*(max-min)/mean());
    }
};

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
    statistics stats;
    for (int trial = 0; trial < n_trials; trial++) {
        init();
        gettimeofday(&start, NULL);
        loop();
        gettimeofday(&end,   NULL);
        double tim = tdiff_timeval(&start, &end);
        stats.measurement(tim);
        printf(" %12.9fs\n", tim);
    }
    stats.print();
}

void timeit_clock_gettime(clockid_t clk_id, const char *string) {
    struct timespec start, end;
    printf("%s\n", string);
    statistics stats;
    for (int trial = 0; trial < n_trials; trial++) {
        init();
        clock_gettime(clk_id, &start);
        loop();
        clock_gettime(clk_id, &end);
        double tim = tdiff_timespec(&start, &end);
        stats.measurement(tim);
        printf(" %12.9fs\n", tim);
    }
    stats.print();
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
