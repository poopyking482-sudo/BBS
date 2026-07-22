#ifndef USERLIB_H
#define USERLIB_H

#include <stdint.h>

#define MAX_USERNAME 32
#define MAX_USERS 16

typedef enum {
    ROLE_GUEST = 0,
    ROLE_USER  = 1,
    ROLE_ADMIN = 2,
    ROLE_ROOT  = 3
} user_role_t;

typedef struct {
    uint32_t uid;
    uint32_t gid;
    char username[MAX_USERNAME];
    uint32_t password_hash; // Simple hash check for bare-metal execution
    user_role_t role;
    char home_dir[64];
    char chroot_path[64];
} user_profile_t;

typedef struct {
    user_profile_t current_user;
    uint8_t is_authenticated;
    uint32_t session_id;
} user_session_t;

void userlib_init(void);
int userlib_add_user(const char *name, const char *pass, user_role_t role, const char *root_path);
user_profile_t* userlib_get_current_user(void);
int userlib_authenticate(const char *username, const char *password);
int userlib_switch_context(uint32_t uid);
void userlib_logout(void);
uint32_t userlib_hash_password(const char *pass);

#endif // USERLIB_H
