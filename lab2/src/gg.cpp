#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <atomic>
#include <math.h>
#include <pthread.h>

const int MAX_THREADS = 4; // Максимальное количество потоков
int num_threads = 0; // Фактическое количество потоков

// Матрица коэффициентов и вектор свободных членов
std::vector<std::vector<double>> A;
std::vector<double> b;

// Функция для вывода матрицы
void printMatrix() {
    for (int i = 0; i < A.size(); ++i) {
        for (int j = 0; j < A[i].size(); ++j) {
            std::cout << A[i][j] << " ";
        }
        std::cout << "| " << b[i] << std::endl;
    }
}

// Функция для обработки строк матрицы
void* GaussianElimination(void* arg) {
    int thread_id = *((int*)arg);
    int numRows = A.size();
    int start = (numRows / num_threads) * thread_id;
    int end = (thread_id == num_threads - 1) ? numRows : (start + numRows / num_threads);

    for (int i = start; i < end; i++) {
        // Находим максимальный элемент в столбце
        double maxVal = std::abs(A[i][i]);
        int maxRow = i;
        for (int j = i + 1; j < numRows; j++) {
            if (std::abs(A[j][i]) > maxVal) {
                maxVal = std::abs(A[j][i]);
                maxRow = j;
            }
        }

        // Перестановка строк
        if (maxRow != i) {
            std::swap(A[i], A[maxRow]);
            std::swap(b[i], b[maxRow]);
        }

        for (int j = i + 1; j < numRows; j++) {
            double ratio = A[j][i] / A[i][i];
            for (int k = i; k < numRows; k++) {
                A[j][k] -= ratio * A[i][k];
            }
            b[j] -= ratio * b[i];
        }
    }
    return nullptr;
}

int main() {
    // Заполните матрицу A и вектор b

    // Пример:
    A = {{2, 1, -1},
         {-3, -1, 2},
         {-2, 1, 2}};
    b = {8, -11, -3};

    num_threads = std::min(MAX_THREADS, static_cast<int>(A.size()));
    pthread_t threads[MAX_THREADS];
    int thread_ids[MAX_THREADS];

    // Создание и запуск потоков
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], nullptr, GaussianElimination, &thread_ids[i]);
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }

    // Обратный ход метода Гаусса
    for (int i = A.size() - 1; i >= 0; i--) {
        for (int j = i + 1; j < A.size(); j++) {
            b[i] -= A[i][j] * b[j];
            A[i][j] = 0;
        }
        b[i] /= A[i][i];
        A[i][i] = 1;
    }

    // Вывод результатов
    std::cout << "Решение системы:\n";
    for (int i = 0; i < A.size(); ++i) {
        std::cout << "x[" << i << "] = " << b[i] << std::endl;
    }

    return 0;
}