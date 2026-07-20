#include "init/sys_sched.h"
#include "init/sys_types.h"

extern void switch_context(uint64_t *old_rsp, uint64_t new_rsp);

static uint64_t current_rsp = 0;

void ksched_yield(uint64_t next_rsp) {
    if (next_rsp == 0) return;
    
    uint64_t *old = &current_rsp;
    current_rsp = next_rsp;
    switch_context(old, next_rsp);
}
