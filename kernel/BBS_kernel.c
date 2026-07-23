#include <stdint.h>

extern void kmain(void);

void BBS_kernel(void) {
    kmain();
    while (1) {
        __asm__ volatile("cli; hlt");
    }
}

 