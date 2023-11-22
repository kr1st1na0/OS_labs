#pragma once

#include <iostream>
#include <cmath>

#ifdef __cplusplus
extern "C" {
#endif

const float NUM_POINTS = 3000; // число разбиений
const float PI = 3.1415926535;

float SinIntegral(float A, float B, float e);
int PrimeCount(int A, int B);

#ifdef __cplusplus
}
#endif