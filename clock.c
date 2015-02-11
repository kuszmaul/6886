#define _GNU_SOURCE
#include<time.h>
#include<assert.h>
#include<stdio.h>

int main (int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
    struct timespec start, end;
    {
	int r = clock_gettime(CLOCK_MONOTONIC, &start);
	assert(r==0);
    }
    int N = 10000000;
    for (int i=0; i<N; i++) {
	struct timespec now;
	int r = clock_gettime(CLOCK_MONOTONIC, &now);
	assert(r==0);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double tdiff = ((end.tv_sec-start.tv_sec)
		    + 1e-9*(end.tv_nsec-start.tv_nsec));
    printf("Time %.6fs (%gns per call to clock_gettime(CLOCK_MONOTONIC,...)\n",
	   tdiff, 1e9*(tdiff/(double)N));
    return 0;
}
    


