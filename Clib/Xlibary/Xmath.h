#ifndef XMATH_H
#define XMATH_H

// high-precision Pi
#define X_PI 3.14159265358979323846L

// simple 2D coordinate structures for games or screen layouts
typedef struct {
    int x;
    int y;
} XPoint;

// absolute value for long doubles
static inline long double Xfabsl(long double x) {
    return (x < 0.0L) ? -x : x;
}

// power function (x^y) for integer exponents
static inline long double Xpowl(long double base, int exp) {
    long double result = 1.0L;
    int positive_exp = (exp < 0) ? -exp : exp;
    
    for (int i = 0; i < positive_exp; i++) {
        result *= base;
    }
    
    return (exp < 0) ? 1.0L / result : result;
}


static inline long double Xsqrtl(long double number) {
    if (number < 0.0L) return 0.0L;
    long double x = number;
    long double y = 1.0L;
    long double e = 0.0000000000001L; // Precision limit
    while (x - y > e) {
        x = (x + y) / 2.0L;
        y = number / x;
    }
    return x;
}

#endif // XMATH_H
