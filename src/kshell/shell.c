#include "kernel/include/vfs.h"
#include "binutils/binutils.h"
#include "kernel/include/kio_ports.h"
#include "kernel/include/ksubsys.h"
#include "kernel/include/kinterrupt.h"
#include "init/include/sys_io.h"
#include "sys_string.h"

#define CMD_BUF_SIZE 128

static char input_buf[CMD_BUF_SIZE];
static uint32_t buf_pos = 0;
static volatile char last_char = 0;

static void shell_prompt(void) {
    io_print("root@bbs-os:~# ");
}


static void read_confirmation(char *out_buf, uint32_t max_len) {
    uint32_t pos = 0;
    while (1) {
        if (last_char == 0) continue;
        char c = last_char;
        last_char = 0;

        if (c == '\n') {
            io_print("\n");
            out_buf[pos] = '\0';
            return;
        } else if (c == '\b') {
            if (pos > 0) {
                pos--;
                io_print("\b \b");
            }
        } else if (pos < max_len - 1) {
            out_buf[pos++] = c;
            char str[2] = {c, '\0'};
            io_print(str);
        }
    }
}

static void parse_and_execute(char *line) {
    if (line[0] == '\0') return;

    char cmd[64] = {0};
    char arg1[64] = {0};
    char arg2[64] = {0};

    uint32_t i = 0, j = 0;
    while (line[i] != ' ' && line[i] != '\0') cmd[j++] = line[i++];
    cmd[j] = '\0';

    if (line[i] == ' ') {
        i++; j = 0;
        while (line[i] != ' ' && line[i] != '\0') arg1[j++] = line[i++];
        arg1[j] = '\0';
    }

    if (line[i] == ' ') {
        i++; j = 0;
        while (line[i] != '\0') arg2[j++] = line[i++];
        arg2[j] = '\0';
    }

    // --- Safety Guards for Core Utilities ---

    // if user deleted root using root account
    if (k_strcmp(cmd, "rm") == 0) {
        if (k_strcmp(arg1, "/") == 0 || k_strcmp(arg1, "-rf /") == 0 || 
            k_strcmp(arg1, "-rf") == 0 || k_strcmp(arg2, "/") == 0) {
            
            io_print("\n\033[1;31m!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\033[0m\n");
            io_print("\033[1;31mCRITICAL WARNING: DESTRUCTIVE ACTION REQUESTED!\033[0m\n");
            io_print("You are attempting to recursively delete the Root Directory ('/').\n");
            io_print("This will destroy the VFS node table, kernel subtrees, and core binaries.\n");
            io_print("\033[1;31mTHIS SYSTEM WILL BE UNBOOTABLE.\033[0m\n");
            io_print("\nType '\033[1;33mYES, DO AS I SAY!\033[0m' to confirm execution: ");

            char confirm[64] = {0};
            read_confirmation(confirm, sizeof(confirm));

            if (k_strcmp(confirm, "YES, DO AS I SAY!") != 0) {
                io_print("\033[1;32m[sh] Aborted root deletion. System preserved.\033[0m\n\n");
                return;
            }

            io_print("\n\033[1;31m[sh] Executing /bin/rm against root filesystem...\033[0m\n");
        }
    }

    // safety prompt for 'mkdir' when target already exists
    if (k_strcmp(cmd, "mkdir") == 0 && arg1[0] != '\0') {
        if (vfs_exists(arg1)) {
            io_print("mkdir: cannot create directory '");
            io_print(arg1);
            io_print("': Directory or node already exists.\n");
            return;
        }
    }

    // SHELL!

    if (k_strcmp(cmd, "help") == 0) {
        io_print("commands\n");
        io_print("  help                 - Display help menu\n");
        io_print("  ls [path]            - List VFS directory contents\n");
        io_print("  pkg <action> <app>   - Package manager CLI\n");
        
    } 
    else if (k_strcmp(cmd, "ls") == 0) {
        vfs_list_dir(arg1[0] ? arg1 : "/");
    } 
    else if (k_strcmp(cmd, "pkg") == 0) {
        app_pkg(arg1, arg2);
    } 
    else if (k_strcmp(cmd, "mininano") == 0) {
        app_mininano(arg1);
    } 
    else {
        // Search FHS PATH locations for core utilities (/bin/rm, /bin/mkdir, etc.)
        const char *system_path[] = { "/bin/", "/usr/bin/", "/sbin/", "/usr/sbin/", 0 };
        int executed = 0;

        for (int p = 0; system_path[p] != 0; p++) {
            char exec_path[VFS_MAX_PATH];
            k_strcpy(exec_path, system_path[p]);
            k_strcat(exec_path, cmd);

            if (vfs_exists(exec_path)) {
                io_print("[sh] executing binary: ");
                io_print(exec_path);
                io_print("\n");
                executed = 1;
                break;
            }
        }

        if (!executed) {
            io_print("sh: command not found: ");
            io_print(cmd);
            io_print("\n");
        }
    }
}

void shell_init(void) {
    register_irq(1, keyboard_irq_handler);
    ksubsys_log(&LOG_INPUT, "keyboard handler attached to IRQ 1");
}

void shell_loop(void) {
    shell_prompt();
    buf_pos = 0;

    while (1) {
        if (last_char == 0) continue;
        char c = last_char;
        last_char = 0;

        if (c == '\n') {
            io_print("\n");
            input_buf[buf_pos] = '\0';
            parse_and_execute(input_buf);
            buf_pos = 0;
            shell_prompt();
        } else if (c == '\b') {
            if (buf_pos > 0) {
                buf_pos--;
                io_print("\b \b");
            }
        } else if (buf_pos < CMD_BUF_SIZE - 1) {
            input_buf[buf_pos++] = c;
            char str[2] = {c, '\0'};
            io_print(str);
        }
    }
}
