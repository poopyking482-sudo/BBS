#ifndef Xnoreturn
    #if defined(__GNUC__) || defined(__clang__)
        #define Xnoreturn __attribute__((__noreturn__))
    #else
        #define Xnoreturn _Noreturn
    #endif
    #define __Xnoreturn_is_defined 1
#endif
