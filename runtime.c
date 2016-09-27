#include <sys/mman.h>
#include <atomics.h>
#include <sched.h>

CASSERT(SLAB_SIZE == PAGE_SIZE);
struct slab *(new_slabs)(cnt batch){
    static struct slab *volatile brk = (struct slab *) HEAP_START;
    struct slab *s;
    dbg cnt loops = 0;
    for(;;){
        s = xadd(SLAB_SIZE * batch, &brk);
        assert(loops++ < 4);
        void *ret = mmap(s, SLAB_SIZE * batch, PROT_WRITE,
                         MAP_PRIVATE | MAP_POPULATE | MAP_ANONYMOUS, -1, 0);
        if(ret == s)
            return s;
        if(ret != MAP_FAILED)
            munmap(ret, SLAB_SIZE * batch);
    }
}

#include <pthread.h>
#include <thread.h>
__thread thread manual_tls;

struct thread *this_thread(void){
    return (struct thread *) pthread_self();
}

__thread uint _dbg_id;
dbg_id get_dbg_id(void){
    return _dbg_id;
}

void set_dbg_id(dbg_id id){
    _dbg_id = id;
}

bool interrupts_enabled(void){
    return true;
}

err kyield(tid t){
    assert(t == -1);
    muste(sched_yield());
    return 0;
}

void *heap_start(){
    return (void *) HEAP_START;
    /* extern void *end; */
    /* return &end; */
}

/* TODO: /proc/self/nonsense */
void *heap_end(){
    return (void *) 0x0101010101010101;
}

void rdtsc(){
    TODO();
}

#include <stdio.h>
#include <signal.h>
inline
void breakpoint(void){
    fflush(stdout);
    raise(SIGABRT);
}

noinline
void panic(const char *_, ...){
    breakpoint();
    abort();
    __builtin_unreachable();
}

bool poisoned(void){
    return false;
}
