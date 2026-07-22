#include "init/include/sys_io.h"
#include "kshell/sys_string.h"

extern char carry_read_char(int file_idx, unsigned int offset);
extern int carry_get_active_index(void);
extern unsigned int carry_get_file_size(int file_idx);

void core_cat(const char *filename) {
    (void)filename;
    int idx = carry_get_active_index();
    if (idx < 0) {
        io_print("cat: no active file\n");
        return;
    }

    unsigned int sz = carry_get_file_size(idx);
    for (unsigned int i = 0; i < sz; i++) {
        char ch[2] = { carry_read_char(idx, i), '\0' };
        io_print(ch);
    }
    io_print("\n");
}
