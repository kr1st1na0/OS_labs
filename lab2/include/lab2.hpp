#pragma once

#include <vector>
#include <cstdlib>
#include <algorithm>
#include <atomic>
#include <math.h>
#include <pthread.h>

using ldbl = long double;

using TVector = std::vector<ldbl>;
using TMatrix = std::vector<TVector>;

struct Args {
    int startRow = 0;
    int endRow = 0;
    TMatrix *lhs = nullptr;
    TVector *rhs = nullptr;
    int leadRow = 0;
};

struct MaxWithRow {
    ldbl value;
    int row;
};

struct ArgsForMax {
    int start = 0;
    int end = 0;
    std::vector<MaxWithRow> *maxElements = nullptr;
    const TMatrix *matrix = nullptr;
    long threadNum = 0;
};


void *MaxElem(void *arguments);
int MaxElemRowParal(const TMatrix &matrix, int start, long threadAmount);
int MaxElemRow(const TMatrix &matrix, int start);
void SwapRows(TMatrix &lhs, TVector &rhs, int first, int second);
void *Normalization(void *arguments);
TVector GaussMethod(long threadAmount, const TMatrix &lhs, const TVector &rhs);