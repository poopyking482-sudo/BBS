#include "include/ktime.h"
#include "include/ksubsys.h"
#include "include/kinterrupt.h"
#include "init/include/sys_io.h"

static volatile unsigned int system_ticks = 0;
static irq_handler_t irq_routines[16] = {0};

// Hardware Timer IRQ 0 Handler
static void timer_irq_handler(interrupt_frame_t *frame) {
    (void)frame;
    system_ticks++;
}

ktime_t ktime_get(void) {
    ktime_t t;
    // Assuming PIT configured for 1000Hz (1ms per tick)
    t.seconds = system_ticks / 1000;
    t.microseconds = (system_ticks % 1000) * 1000;
    return t;
}

xbool register_irq(unsigned int irq, irq_handler_t handler) {
    if (irq >= 16) return XFALSE;
    irq_routines[irq] = handler;
    return XTRUE;
}

void unregister_irq(unsigned int irq) {
    if (irq < 16) irq_routines[irq] = 0;
}

void ksubsys_log(const ksubsys_t *subsys, const char *msg) {
    char time_str[16];
    ktime_format(ktime_get(), time_str);

    io_print(time_str);
    io_print("[");
    io_print(subsys->tag);
    io_print("] ");
    io_print(msg);
    io_print("\n");
}

void ktime_subsys_init(void) {
    register_irq(0, timer_irq_handler);
    ksubsys_log(&LOG_CLOCK, "hardware timer registered on IRQ 0");
    ksubsys_log(&LOG_RTC, "real-time clock subsystem online");
}
