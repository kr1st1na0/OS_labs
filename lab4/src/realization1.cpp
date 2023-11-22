#include "realizations.hpp"

extern "C" float SinIntegral(float A, float B, float e) {
    float integral = 0.0;
    e = (B - A) / NUM_POINTS;
    for (int i = 0; i <= NUM_POINTS; ++i) {
	    integral = integral + e * sin(A + e * (i - 0.5));
    }
    return integral;
}

extern "C" int PrimeCount(int A, int B) {
    int count = 0;
    bool flag = true;
    for (int i = A; i <= B; ++i) {
        if (i == 0 || i == 1) {
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