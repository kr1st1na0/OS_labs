#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <vector>

#include <parent.hpp>

namespace fs = std::filesystem;



// TEST(firstLabTests, emptyTest) {

//     std::vector<std::string> input = {
//         ""
//     }

//     std::vector<std::string> output = {
//         "Error with string:  "
//     }
// }

TEST(firstLabTests, simpleTest) {

    std::string fileName = "file.txt";

    std::vector<std::string> input = {
        "No,",
        "you'll never be alone.",
        "When darkness comes;",
        "I'll light the night with stars",
        "Hear my whispers in the dark!"
    };

    std::vector<std::string> expectedOutput = {
        "ERROR with string: No,",
        "ERROR with string: I'll light the night with stars",
        "ERROR with string: Hear my whispers in the dark!"
    };

    std::stringstream inFile;
    inFile << fileName << std::endl;
    for (std::string line : input) {
        inFile << line << std::endl;
    }

    // std::stringstream outFile;
    std::streambuf* oldInBuf = std::cin.rdbuf(inFile.rdbuf()); // выдает старый буфер
    // std::cout.rdbuf();

    ASSERT_TRUE(fs::exists(getenv("PATH_TO_CHILD")));

    testing::internal::CaptureStdout();

    parentProcess(getenv("PATH_TO_CHILD"));
    std::cin.rdbuf(oldInBuf); // чтобы cin вернулся обратно
    
    std::stringstream errorOut(testing::internal::GetCapturedStdout());
    for(std::string expectation : expectedOutput) {
        std::string result;
        getline(errorOut, result);
        EXPECT_EQ(result, expectation);
    }
}

int main(int argc, char *argv[]) {
    std::cout << getenv("PATH_TO_CHILD") << std::endl;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}