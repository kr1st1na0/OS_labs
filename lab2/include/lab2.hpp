#pragma once

#include <vector>
#include <cstdlib>
#include <algorithm>
#include <atomic>
#include <math.h>
#include <pthread.h>

using TVector = std::vector<double>;
using TMatrix = std::vector<TVector>;

struct Args {
    int startRow = 0;
    int endRow = 0;
    TMatrix *lhs = nullptr;
    TVector *rhs = nullptr;
    int leadRow;
};


void printMatrix(const TMatrix &matrix, const TVector &vector);
int maxElemRow(const TVector &tRow, int start);
void swapRows(TMatrix &lhs, TVector &rhs, int first, int second);
void *normalization(void *arguments);
void gaussMethod(long threadAmount, TMatrix &lhs, TVector &rhs, TVector &answer);