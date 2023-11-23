#pragma once

#include <iostream>
#include <cmath>

#ifdef __cplusplus
extern "C" {
#endif

constexpr int NUM_POINTS = 3000; // число разбиений
const float PI = 3.1415926535;

float SinIntegral(float a, float b, float e);
int PrimeCount(int a, int b);

#ifdef __cplusplus
}
#endif