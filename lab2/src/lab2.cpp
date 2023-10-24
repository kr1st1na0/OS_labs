#include "lab2.hpp"
#include <iostream>

void printMatrix(const TMatrix &matrix, const TVector &vector) { // for check
    int matrixSize = matrix.size();
    for (int n = 0; n < matrixSize; ++n) {
        for (int j = 0; j < matrixSize; ++j) {
            std::cout << matrix[n][j] << ' ';
        }
        std::cout << "| " << vector[n] << std::endl;
    }
}

int maxElemRow(const TMatrix &matrix, int start) {
    int matrixSize = matrix.size();
    double maxElem = fabs(matrix[start][start]);
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

void swapRows(TMatrix &lhs, TVector &rhs, int first, int second) {
    lhs[first].swap(lhs[second]);
    std::swap(rhs[first], rhs[second]);
}

void *normalization(void *arguments) {
    const auto &args = *(reinterpret_cast<Args *>(arguments));
    auto &leftMatrix = *args.lhs;
    auto &rigthVector = *args.rhs;
    auto &leadRow = args.leadRow;
    auto &startRow = args.startRow;
    auto &endRow = args.endRow;
    int matrixSize = leftMatrix.size();
    for (int i = leadRow + 1; i >= startRow && i < endRow; ++i) {
        double coef = -leftMatrix[i][leadRow] / leftMatrix[leadRow][leadRow];
        leftMatrix[i][leadRow] = 0.0;
        for (int j = leadRow + 1; j < matrixSize; ++j) {
            leftMatrix[i][j] += leftMatrix[leadRow][j] * coef;
        }
        rigthVector[i] += rigthVector[leadRow] * coef;
    }
    return nullptr;
}

void gaussMethod(long threadAmount, TMatrix &lhs, TVector &rhs, TVector &answer) {
    long matrixSize = lhs.size();
    int leadRow = 0;
    threadAmount = std::min(threadAmount, matrixSize);
    std::vector<pthread_t> threads(threadAmount);
    const long rowsPerThread = matrixSize / threadAmount;
    std::vector<Args> threadArgs(threadAmount);
    for (int i = 0; i < matrixSize; ++i) {
        leadRow = maxElemRow(lhs, leadRow);
        double leadElem = lhs[leadRow][i];
        for (int k = 0; k < matrixSize; ++k) {
            lhs[leadRow][k] /= leadElem;
        }
        rhs[leadRow] /= leadElem;
        if (leadRow != i) {
            swapRows(lhs, rhs, i, leadRow);
        } else {
            ++leadRow;
        }
        for (int n = 0; n < threadAmount; ++n) {   
            threadArgs[n].startRow = n * rowsPerThread;
            threadArgs[n].endRow = threadArgs[n].startRow + rowsPerThread;
            threadArgs[n].lhs = &lhs;
            threadArgs[n].rhs = &rhs;
            threadArgs[n].leadRow = i;
            if ((n + 1) * rowsPerThread >= matrixSize) {
                threadArgs[n].endRow = matrixSize - n * rowsPerThread;
                pthread_create(&threads[n], nullptr, normalization, reinterpret_cast<void *>(&threadArgs[n]));  
            }
            else {
                pthread_create(&threads[n], nullptr, normalization, reinterpret_cast<void *>(&threadArgs[n]));
            }
        }
        for (auto &thread : threads) {
            pthread_join(thread, nullptr);
        }
    }
    // Reverse move
    for (int k = matrixSize - 1; k >= 0; --k) {
        answer[k] = rhs[k];
        for (int i = 0; i < k; ++i) {
            rhs[i] = rhs[i] - lhs[i][k] * answer[k];
        }
    }
}
