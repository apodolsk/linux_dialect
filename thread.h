#pragma once

#include <nalloc.h>
#include <wkrand.h>

typedef struct thread{
    nalloc_tls nallocin;
    rand_tls randin;
    bool racing;
} thread;

extern __thread thread manual_tls;
#define T (&manual_tls)
