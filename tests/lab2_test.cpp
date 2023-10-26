#include <gtest/gtest.h>

#include "lab2.hpp"

#include <limits>
#include <chrono>

const ldbl LDBL_PRECISION = 0.0001;

namespace {
    TMatrix GenerateMatrix(int n) {
        TMatrix result(n, TVector(n));
        std::srand(std::time(nullptr));
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                result[i][j] = std::rand() % 100;
            }
        }
        return result;
    }

    TVector GenerateVector(int n) {
        TVector result(n);
        std::srand(std::time(nullptr));
        for(int i = 0; i < n; ++i) {
            result[i] = std::rand() % 100;
        }
        return result;
    }
}

bool AreEqual(const TVector &first, const TVector &second) {
    if (first.size() != second.size()) {
        return false;
    }
    for(size_t i = 0; i < first.size(); ++i) {
        if (!(std::fabs(first[i] - second[i]) < LDBL_PRECISION)) {
            return false;
        }
    }
    return true;
}

TEST(SecondLabTests, SingleThreadYieldsCorrectResults) {
    ASSERT_TRUE( AreEqual(GaussMethod(1, TMatrix{{1, -1}, {2, 1}}, TVector{-5, -7}), TVector{-4, 1}) );
    
    ASSERT_TRUE( AreEqual(GaussMethod(1, TMatrix{{2, 4, 1}, {5, 2, 1}, {2, 3, 4}}, TVector{36, 47, 37}), TVector{7, 5, 2}) );

    ASSERT_TRUE( AreEqual(GaussMethod(1, TMatrix{{3, 2, -5}, {2, -1, 3}, {1, 2, -1}}, TVector{-1, 13, 9}), TVector{3, 5, 4}) );

    ASSERT_TRUE( AreEqual(GaussMethod(1, TMatrix{{1, 1, 2, 3}, {1, 2, 3, -1}, {3, -1, -1, -2}, {2, 3, -1, -1}}, TVector{1, -4, -4, -6}),  
    TVector{-1, -1, 0, 1}) );
}

TEST(SecondLabTest, ThreadConfigurations) {
    auto performTestForGivenSize = [](int n, int maxThreadCount) {
        auto m = GenerateMatrix(n);
        auto v = GenerateVector(n);
        auto result = GaussMethod(1, m, v);

        for(int i = 2; i < maxThreadCount; ++i) {
            ASSERT_TRUE( AreEqual(GaussMethod(i, m, v), result) );
        }
    };

    performTestForGivenSize(3, 10);
    performTestForGivenSize(10, 10);
    performTestForGivenSize(100, 15);
    performTestForGivenSize(1000, 4);
}

TEST(SecondLabTest, PerfomanceTest) {
    auto getAvgTime = [](int threadCount) {
        auto m = GenerateMatrix(3000);
        auto v = GenerateVector(3000);

        constexpr int runsCount = 1;

        double avg = 0;

        for(int i = 0; i < runsCount; ++i) {
            auto begin = std::chrono::high_resolution_clock::now();
            GaussMethod(threadCount, m, v);
            auto end = std::chrono::high_resolution_clock::now();
            avg += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        }

        return avg / runsCount;
    };

    auto singleThread = getAvgTime(1);
    auto multiThread = getAvgTime(4);

    std::cout << "Avg time for 1 thread: " << singleThread << '\n';
    std::cout << "Avg time for 4 threads: " << multiThread << '\n';

    EXPECT_GE(singleThread, multiThread);
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);

    std::cout.precision(15);
    std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);

    return RUN_ALL_TESTS();
}