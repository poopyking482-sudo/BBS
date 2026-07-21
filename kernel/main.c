#include <stdint.h>
#include "include/vfs.h"
#include "include/Kinterrupt.h"
#include "include/Kio_ports.h"
#include "include/Ksubsys.h"
#include "include/Ktime.h"

// driver for vga
static volatile uint16_t* VGA_BUFFER = (uint16_t*)0xB8000;
static int vga_col = 0;
static int vga_row = 0;

void io_print(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            vga_col = 0;
            vga_row++;
            if (vga_row >= 25) vga_row = 0; // simple wrap
            continue;
        }
        VGA_BUFFER[vga_row * 80 + vga_col] = (uint16_t)str[i] | (0x0F << 8);
        vga_col++;
        if (vga_col >= 80) {
            vga_col = 0;
            vga_row++;
        }
    }
}

// LOGS
void klog(const ksubsys_t subsys, const char* msg) {
    char time_str[15];
    ktime_t now = ktime_get();
    ktime_format(now, time_str);

    io_print(time_str);
    io_print("[");
    io_print(subsys.tag);
    io_print("] ");
    io_print(msg);
    io_print("\n");
}

// Virtual file system
static int console_vfs_write(vfs_node_t *node, uint32_t offset, const void *buffer, uint32_t size) {
    (void)node; (void)offset;
    const char *char_buf = (const char *)buffer;
    for (uint32_t i = 0; i < size; i++) {
        outb(0x3F8, char_buf[i]); // Emit to COM1 serial port
    }
    return size;
}

static vfs_ops_t console_ops = {
    .read = 0,
    .write = console_vfs_write
};

void init_vfs_devices(void) {
    vfs_node_t dev_console = {
        .name = "/dev/console",
        .size = 0,
        .is_dir = 0,
        .inode_id = 1,
        .ops = &console_ops
    };
    vfs_register_node(dev_console);
}

// ktime driver stub
static uint32_t g_ticks = 0;
ktime_t ktime_get(void) {
    g_ticks++;
    ktime_t t;
    t.seconds = g_ticks / 1000;
    t.microseconds = (g_ticks % 1000) * 1000;
    return t;
}

// launch INIT
void init_main(void);

void kmain(void) {
    // clear VGA terminal
    for (int i = 0; i < 80 * 25; i++) {
        VGA_BUFFER[i] = (uint16_t)' ' | (0x0F << 8);
    }

    klog(LOG_CLOCK, "timer subsystem initialized");
    klog(LOG_SERIO, "mounting VFS node /console...");
    
    init_vfs_devices();

    klog(LOG_INPUT, "handing over execution to core supervisor (init)...");
    
    // Jump to your supervisor loop!
    init_main();
}
