#ifndef XCALL_H
#define XCALL_H



// we just declare the signatures. the environment/linker resolves them.
extern int sys_spawn(const char* path, const char* args[]);
extern void sys_exit(int code);

// Is that success or error
static inline int call(const char* binary_path, const char* args[]) {
    if (binary_path == 0) {
        return -1; 
    }
    return sys_spawn(binary_path, args);
}

// "void terminate"
static inline void terminate(int status) {
    sys_exit(status);
}

#endif // XCALL_H
