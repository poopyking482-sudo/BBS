#include "carry.h"

extern char kbd_getchar(void);
extern void serial_write(const char *s);

void launch_mininano(void) {
    static int initialized = 0;
    if (!initialized) {
        carry_init();
        initialized = 1;
    }

    mininano_render();

    while (1) {
        char c = kbd_getchar();

        if (c == 0x11) {
            serial_write("\033[2J\033[H");
            serial_write("Exited Mininano.\n");
            break;
        }

        mininano_handle_input(c);
    }
}
