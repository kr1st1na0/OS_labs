#include "realizations.hpp"
#include <vector>

extern "C" float SinIntegral(float a, float b, float e) {
    float integral = 0.0;
    e = (b - a) / NUM_POINTS;
    for (int i = 1; i < NUM_POINTS; ++i) {
        float x1 = a + i * e;
        float x2 = a + (i + 1) * e;
        integral += 0.5 * e * (sin(x1) + sin(x2));
    }
    return integral;
}

extern "C" int PrimeCount(int a, int b) {
    int count = 0;
    std::vector<int> numbers;
    numbers.reserve(b + 1);
    for (int i = 0; i <= b; ++i) {
        numbers.push_back(i);
    }
    for (int i = 2; i <= b; ++i) {
        if (numbers[i] != 0) {
            if (numbers[i] >= a && numbers[i] <= b) {
                ++count;
            }
            for (int j = i * i; j <= b; j += i) {
                numbers[j] = 0;
            }
        }
    }
    return count;
}