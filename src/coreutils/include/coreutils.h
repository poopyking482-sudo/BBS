#ifndef COREUTILS_H
#define COREUTILS_H

// system utilities
void core_echo(const char *args);
void core_ls(const char *args);
void core_cat(const char *filename);
void core_grep(const char *pattern, const char *text);

// password utilities 
void core_sudo(const char *cmd, const char *arg, const char *password);
void core_passwd(const char *target_user, const char *old_or_new_pass, const char *new_pass_if_root);

#endif // COREUTILS_H
