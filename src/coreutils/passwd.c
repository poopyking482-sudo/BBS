#include "init/include/sys_io.h"
#include "kshell/sys_string.h"
#include "lib/include/userlib.h"
#include "lib/include/passwdlib.h"

static passwd_entry_t passwd_db[MAX_ENTRIES];
static shadow_entry_t shadow_db[MAX_ENTRIES];
static uint32_t entry_count = 0;

void passwdlib_init(void) {
    entry_count = 0;

    // Seed Root account (/etc/passwd & /etc/shadow)
    k_strncpy(passwd_db[0].username, "root", 32);
    passwd_db[0].uid = 0;
    passwd_db[0].gid = 0;
    k_strncpy(passwd_db[0].home, "/", 64);
    k_strncpy(passwd_db[0].shell, "/bin/sh", 32);

    k_strncpy(shadow_db[0].username, "root", 32);
    shadow_db[0].pass_hash = userlib_hash_password("rootpass");

    // Seed Standard user account
    k_strncpy(passwd_db[1].username, "bbsuser", 32);
    passwd_db[1].uid = 1000;
    passwd_db[1].gid = 1000;
    k_strncpy(passwd_db[1].home, "/home/bbsuser", 64);
    k_strncpy(passwd_db[1].shell, "/bin/sh", 32);

    k_strncpy(shadow_db[1].username, "bbsuser", 32);
    shadow_db[1].pass_hash = userlib_hash_password("bbs123");

    entry_count = 2;
}

int passwdlib_lookup_uid(const char *username) {
    for (uint32_t i = 0; i < entry_count; i++) {
        if (k_strcmp(passwd_db[i].username, username) == 0) {
            return passwd_db[i].uid;
        }
    }
    return -1;
}

int passwdlib_verify_password(const char *username, const char *password) {
    uint32_t hash = userlib_hash_password(password);
    for (uint32_t i = 0; i < entry_count; i++) {
        if (k_strcmp(shadow_db[i].username, username) == 0) {
            return (shadow_db[i].pass_hash == hash) ? 0 : -1;
        }
    }
    return -1;
}

int passwdlib_update_password(const char *username, const char *new_pass) {
    uint32_t new_hash = userlib_hash_password(new_pass);
    for (uint32_t i = 0; i < entry_count; i++) {
        if (k_strcmp(shadow_db[i].username, username) == 0) {
            shadow_db[i].pass_hash = new_hash;
            return 0; // good job!
        }
    }
    return -1; // What's that? error?
}
