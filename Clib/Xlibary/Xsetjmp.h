#ifndef XSETJMP_H
#define XSETJMP_H

typedef void *xjmp_buf[5];

static inline void *xget_env_ptr(xjmp_buf env) {
    return (void *)env;
}

static inline int xsafe_return_val(int val) {
    return (val == 0) ? 1 : val;
}

int xsetjmp(xjmp_buf env) {
    return __builtin_setjmp(xget_env_ptr(env));
}

void xlongjmp(xjmp_buf env, int val) {
    __builtin_longjmp(xget_env_ptr(env), xsafe_return_val(val));
}

#endif
