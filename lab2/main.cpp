#include <iostream>

#include "lab2.hpp"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Not enough arguments" << std::endl;
        return -1;
    }

    long threadAmount = std::atol(argv[1]);

    int n;
    std::cin >> n;

    TMatrix lhs(n, TVector(n));
    TVector rhs(n);

    auto readMatrix = [n](TMatrix &matrix) {
        for (int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                std::cin >> matrix[i][j];
            }
        }
    };

    auto readVector = [n](TVector &vector) {
        for (int i = 0; i < n; ++i) {
            std::cin >> vector[i];
        }
    };

    auto printMatrix = [n](TMatrix &matrix, TVector &vector) {
        for (int i = 0; i < n; ++i) {
            std::cout << "| ";
            for (int j = 0; j < n; ++j) {
                std::cout << matrix[i][j] << ' ';
            }
            std::cout << '|';
            std::cout << " | x" << i + 1 << " | | " << vector[i] << " |" << std::endl;
        }
    };

    readMatrix(lhs);
    readVector(rhs);
    std::cout << "The system of equations in matrix:" << std::endl;
    printMatrix(lhs, rhs);

    auto answer = GaussMethod(threadAmount, lhs, rhs);
    std::cout << "The solution:" << std::endl;
    for (int i = 0; i < n; ++i) {
        std::cout << "x" << i + 1 << " = " << answer[i] << std::endl; 
    }

    return 0;
}