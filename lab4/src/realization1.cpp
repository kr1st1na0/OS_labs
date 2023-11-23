#include "realizations.hpp"

extern "C" float SinIntegral(float a, float b, float e) {
    float integral = 0.0;
    e = (b - a) / NUM_POINTS;
    for (int i = 0; i <= NUM_POINTS; ++i) {
        integral = integral + e * sin(a + e * (i - 0.5));
    }
    return integral;
}

extern "C" int PrimeCount(int a, int b) {
    int count = 0;
    bool flag = true;
    for (int i = a; i <= b; ++i) {
        if (i <= 1) {
            continue;
        }
        for (int j = 2; j < i; ++j) {
            if (i % j == 0) {
                flag = false;
                break;
            }
        }
        if (flag) {
            ++count;
        }
        flag = true;
    }
    return count;
}