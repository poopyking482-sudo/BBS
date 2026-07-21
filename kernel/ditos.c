#include "ditos.h"

typedef void (*ditos_entry_t)(void);

void bbs_boot_entry(void) {
    io_print("decompressing krnl...");

    int status = decompress(input_data, input_len, DITOS_KERNEL_LOAD_ADDR);

    if (status != 0) {
        io_print("failed!\n[ditos] CRIT! fault, CPU has halted.\n");
        while (1) {
            asm volatile ("hlt");
        }
    }

    io_print("OK\n");
    io_print("starting krnl...\n\n");

    // 2. Jump to uncompressed kernel entry point
    bbs_entry_t start_bbs = (bbs_entry_t)BBS_KERNEL_LOAD_ADDR;
    start_bbs();
}
