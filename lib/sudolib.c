#include "init/include/sys_io.h"
#include "include/sys_string.h"
#include "include/sudolib.h"
#include "include/userlib.h"

static sudo_rule_t sudoers_table[MAX_SUDOERS];
static uint32_t sudoers_count = 0;
static uint8_t elevated_flag = 0;
static user_profile_t pre_elevation_user;

void sudolib_init(void) {
    sudoers_count = 0;
    elevated_flag = 0;

    // be root or be user
    sudolib_add_rule(1000, 1, "/");
}

int sudolib_add_rule(uint32_t uid, uint8_t allow_all, const char *allowed_chroot) {
    if (sudoers_count >= MAX_SUDOERS) return -1;

    sudoers_table[sudoers_count].uid = uid;
    sudoers_table[sudoers_count].allow_all = allow_all;

    uint32_t i = 0;
    while (allowed_chroot && allowed_chroot[i] && i < 63) {
        sudoers_table[sudoers_count].restricted_chroot[i] = allowed_chroot[i];
        i++;
    }
    sudoers_table[sudoers_count].restricted_chroot[i] = '\0';

    sudoers_count++;
    return 0;
}

int sudolib_check_permission(uint32_t uid, const char *target_chroot) {
    if (uid == 0) return 1; // Root always allowed

    for (uint32_t i = 0; i < sudoers_count; i++) {
        if (sudoers_table[i].uid == uid) {
            if (sudoers_table[i].allow_all) return 1;
            if (k_strcmp(sudoers_table[i].restricted_chroot, target_chroot) == 0) return 1;
        }
    }
    return 0;
}

int sudolib_elevate(const char *password) {
    user_profile_t *active = userlib_get_current_user();

    if (active->uid == 0) {
        elevated_flag = 1;
        return 0; // active
    }

    if (!sudolib_check_permission(active->uid, "/")) {
        io_print("[sudo] wrong permissions, maybe try again as sudo... logging\n");
        return -1;
    }

    uint32_t pass_hash = userlib_hash_password(password);
    if (pass_hash != active->password_hash) {
        io_print("[sudo] password invalid\n");
        return -2;
    }

    // root
    pre_elevation_user = *active;
    userlib_switch_context(0); // Switch active session UID to 0
    elevated_flag = 1;

    return 0;
}

void sudolib_drop_privileges(void) {
    if (elevated_flag) {
        userlib_switch_context(pre_elevation_user.uid);
        elevated_flag = 0;
    }
}

int sudolib_is_elevated(void) {
    return elevated_flag;
}
