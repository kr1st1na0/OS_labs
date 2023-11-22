#include "realizations.hpp"
#include <vector>

extern "C" float SinIntegral(float A, float B, float e) {
    float integral = 0.0;
    e = (B - A) / NUM_POINTS;
    for (int i = 1; i < NUM_POINTS; ++i) {
        float x1 = A + i * e;
        float x2 = A + (i + 1) * e;
        integral += 0.5 * e * (sin(x1) + sin(x2));
    }
    return integral;
}

extern "C" int PrimeCount(int A, int B) {
    int count = 0;
    std::vector<int> numbers;
    for (int i = 0; i <= B; ++i) {
        numbers.push_back(i);
    }
    for (int i = 2; i <= B; ++i) {
        if (numbers[i] != 0) {
            if (numbers[i] >= A && numbers[i] <= B) {
                ++count;
            }
            for (int j = i * i; j <= B; j += i) {
                numbers[j] = 0;
            }
        }
    }
    return count;
}