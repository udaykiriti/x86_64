#include "math.h"

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int factorial(int n) {
    if (n < 0 || n > 12) return 0;
    if (n <= 1) return 1;

    int res = 1;

    for (int i = 2; i <= n; i++) {
        res *= i;
    }

    return res;
}
