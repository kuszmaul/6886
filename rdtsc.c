#define _GNU_SOURCE
#include<time.h>
#include<assert.h>
#include<stdio.h>

static __inline__ unsigned long long rdtsc(void)
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return (   ((unsigned long long)lo)
	  | (((unsigned long long)hi)<<32));
}

int main (int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
    struct timespec start, end;
    {
	int r = clock_gettime(CLOCK_MONOTONIC, &start);
	assert(r==0);
    }
    int N = 10000000;
    unsigned long long vv=0;
    for (int i=0; i<N; i++) {
	vv += rdtsc();
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double tdiff = ((end.tv_sec-start.tv_sec)
		    + 1e-9*(end.tv_nsec-start.tv_nsec));
    printf("vv=%llu\n", vv);
    printf("Time %.6fs (%gns per call to clock_gettime(CLOCK_MONOTONIC,...)\n",
	   tdiff, 1e9*(tdiff/(double)N));
    return 0;
}
    


