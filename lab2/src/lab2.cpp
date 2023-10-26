#include <iostream>

#include "lab2.hpp"

void *MaxElem(void *arguments) {
    const auto &args = *(reinterpret_cast<ArgsForMax *>(arguments));
    auto start = args.start;
    auto end = args.end;
    auto &maxElements = *args.maxElements;
    auto &matrix = *args.matrix;
    auto &threadNum = args.threadNum;
    ldbl maxElem = fabs(matrix[start][start]);
    int row = start;
    for (int i = start; i < end; ++i) {
        if (fabs(matrix[i][start]) > maxElem) {
            maxElem = fabs(matrix[i][start]);
            row = i;
        }
    }
    if (maxElem == 0) {
        maxElements[threadNum] = {0, -1};
        return nullptr;
    }
    maxElements[threadNum] = {maxElem, row};
    return nullptr;
} 

int MaxElemRowParal(const TMatrix &matrix, int start, long threadAmount) {
    std::vector<ArgsForMax> threadArgs(threadAmount);
    long threadAmountPerIter = std::min(threadAmount, (long)(matrix.size() - start));
    if (threadAmountPerIter == 0) {
        return start;
    }
    long rowsPerThread = std::max(1L, (long)(((matrix.size()) - start) / threadAmountPerIter));
    std::vector<MaxWithRow> maxElements(threadAmountPerIter);
    ldbl absoluteMax = fabs(matrix[start][start]);
    int row = start;
    std::vector<pthread_t> threads(threadAmountPerIter);
    for (long n = 0; n < threadAmountPerIter; ++n) {   
        threadArgs[n].start = start + n * rowsPerThread;
        threadArgs[n].end = (n == threadAmountPerIter - 1) ? matrix.size() : (threadArgs[n].start + rowsPerThread);
        threadArgs[n].maxElements = &maxElements;
        threadArgs[n].matrix = &matrix;
        threadArgs[n].threadNum = n;
        pthread_create(&threads[n], nullptr, MaxElem, reinterpret_cast<void *>(&threadArgs[n]));
    }
    for (auto &thread : threads) {
        pthread_join(thread, nullptr);
    }
    for (int i = 0; i < threadAmountPerIter; ++i) {
        if (maxElements[i].value > absoluteMax) {
            absoluteMax = maxElements[i].value;
            row = maxElements[i].row;
        }
    }
    return row;
}

int MaxElemRow(const TMatrix &matrix, int start) {
    int matrixSize = matrix.size();
    ldbl maxElem = fabs(matrix[start][start]);
    int row = start;
    for (int i = start; i < matrixSize; ++i) {
        if (fabs(matrix[i][start]) > maxElem) {
            maxElem = fabs(matrix[i][start]);
            row = i;
        }
    }
    if (maxElem == 0) {
        return -1;
    }
    return row;
}

void SwapRows(TMatrix &lhs, TVector &rhs, int first, int second) {
    lhs[first].swap(lhs[second]);
    std::swap(rhs[first], rhs[second]);
}

void *Normalization(void *arguments) {
    const auto &args = *(reinterpret_cast<Args *>(arguments));
    auto startRow = args.startRow;
    auto endRow = args.endRow;
    auto &leftMatrix = *args.lhs;
    auto &rightVector = *args.rhs;
    auto leadRow = args.leadRow;
    int matrixSize = leftMatrix.size();
    for (int i = startRow; i < endRow; ++i) {
        ldbl coef = -leftMatrix[i][leadRow] / leftMatrix[leadRow][leadRow];
        leftMatrix[i][leadRow] = 0.0;
        for (int j = leadRow + 1; j < matrixSize; ++j) {
            leftMatrix[i][j] += leftMatrix[leadRow][j] * coef;
        }
        rightVector[i] += rightVector[leadRow] * coef;
    }
    return nullptr;
}

TVector GaussMethod(long threadAmount, const TMatrix &Mlhs, const TVector &Vrhs) {
    TMatrix lhs = Mlhs;
    TVector rhs = Vrhs;

    long matrixSize = lhs.size();
    int leadRow = 0;
    threadAmount = std::min(threadAmount, matrixSize);
    std::vector<Args> threadArgs(threadAmount);
    for (int i = 0; i < matrixSize; ++i) {
        // Parallelization for max elem
        leadRow = (threadAmount > 1) ? MaxElemRowParal(lhs, leadRow, threadAmount) : MaxElemRow(lhs, leadRow);
        if (leadRow == -1) {
            std::cout << "Unable to get the solution due to zero column" << std::endl;
            return {0};
        }
        // Leading string conversion
        ldbl leadElem = lhs[leadRow][i];
        for (int k = 0; k < matrixSize; ++k) {
            lhs[leadRow][k] /= leadElem;
        }
        rhs[leadRow] /= leadElem;
        // Swap rows
        if (leadRow != i) {
            SwapRows(lhs, rhs, i, leadRow);
        } else {
            ++leadRow;
        }
        // Parallelization for strings
        if (threadAmount > 1) {
            if (i != matrixSize - 1) {
                long threadAmountPerIter = std::min(threadAmount, matrixSize - i) - 1;
                long rowsPerThread = std::max(1L, (matrixSize - 1 - i) / threadAmountPerIter);
                std::vector<pthread_t> threads(threadAmountPerIter);
                for (int n = 0; n < threadAmountPerIter; ++n) {   
                    threadArgs[n].startRow = i + 1 + n * rowsPerThread; 
                    threadArgs[n].endRow = (n == threadAmountPerIter - 1) ? matrixSize : (threadArgs[n].startRow + rowsPerThread);
                    threadArgs[n].lhs = &lhs;
                    threadArgs[n].rhs = &rhs;
                    threadArgs[n].leadRow = i;
                    pthread_create(&threads[n], nullptr, Normalization, reinterpret_cast<void *>(&threadArgs[n]));
                }
                for (auto &thread : threads) {
                    pthread_join(thread, nullptr);
                }
            }
        } else {
            threadArgs[0].startRow = i + 1; 
            threadArgs[0].endRow = matrixSize;
            threadArgs[0].lhs = &lhs;
            threadArgs[0].rhs = &rhs;
            threadArgs[0].leadRow = i;
            Normalization(&threadArgs[0]);
        }
    }
    // Reverse move
    TVector answer(matrixSize);
    for (int k = matrixSize - 1; k >= 0; --k) {
        answer[k] = rhs[k];
        for (int i = 0; i < k; ++i) {
            rhs[i] = rhs[i] - lhs[i][k] * answer[k];
        }
    }
    return answer;
}
