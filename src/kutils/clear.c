#include <stdint.h>

#define VGA_MEM ((uint16_t*)0xB8000)
#define VGA_W 80
#define VGA_H 25
#define VGA_ATTR 0x07

extern uint8_t cursor_x;
extern uint8_t cursor_y;

void clear_screen(void) {
    uint16_t blank = ' ' | (VGA_ATTR << 8);

    for (int i = 0; i < VGA_W * VGA_H; i++) {
        VGA_MEM[i] = blank;
    }

    cursor_x = 0;
    cursor_y = 0;
}


// Kernels are surprisingly tiny at their core!
