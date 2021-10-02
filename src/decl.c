#include "decl.h"

int r(int x) {
    return 255 - 2 * abs(128 - 8 * (x % 32));
}

int g(int x) {
    return 255 - 2 * abs(128 - 8 * (x % 32));
}

int b(int x) {
    return 255 - 2 * abs(128 - 8 * (x % 32));
}

double complex f(double complex z, double complex c) {
    return z * z + c;
}