#include <time.h>

/* #define CLOCK_TYPE CLOCK_PROCESS_CPUTIME_ID */
#define CLOCK_TYPE CLOCK_REALTIME_COARSE

static inline dptr wall_ns(void){
    struct timespec t;                                      \
    muste(clock_gettime(CLOCK_REALTIME_COARSE, &t));        \
    return t.tv_sec * 1000000000 + t.tv_nsec;
}
                 
#define JOB_CPU_TIME(expr)                                          \
    ({                                                              \
        struct timespec _start;                                     \
        muste(clock_gettime(CLOCK_TYPE, &_start));                  \
        (expr);                                                     \
        struct timespec _end;                                       \
        muste(clock_gettime(CLOCK_TYPE, &_end));                    \
        1000 * (_end.tv_sec - _start.tv_sec) +                      \
               (_end.tv_nsec - _start.tv_nsec) / 1000000;           \
    })                                                              \

#define TIME(expr) JOB_CPU_TIME(expr)
#define REPORT_TIME(expr) lprintf( #expr ": % ms", TIME(expr));

static inline struct timespec job_get_time(void){
    struct timespec start;
    muste(clock_gettime(CLOCK_REALTIME, &start));
    return start;
}

static inline udptr job_time_diff(struct timespec start){
    struct timespec end;
    muste(clock_gettime(CLOCK_REALTIME, &end));
    return (end.tv_sec - start.tv_sec) * 1000 +
           (end.tv_nsec - start.tv_nsec) / 1000000;
}
