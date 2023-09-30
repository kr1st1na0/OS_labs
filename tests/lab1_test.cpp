#include <gtest/gtest.h>

#include <filesystem>
#include <memory>
#include <vector>

#include <parent.hpp>

namespace fs = std::filesystem;

void testingProgram(std::vector<std::string> input, std::vector<std::string> expectedOutput) {

    std::stringstream inFile;
    inFile << "file.txt" << std::endl;
    for (std::string line : input) {
        inFile << line << std::endl;
    }

    std::streambuf* oldInBuf = std::cin.rdbuf(inFile.rdbuf()); // выдает старый буфер

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

TEST(firstLabTests, emptyTest) {
    std::vector<std::string> input = {};

    std::vector<std::string> expectedOutput = {};

    testingProgram(input, expectedOutput);
}

TEST(firstLabTests, simpleTest) {
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

    testingProgram(input, expectedOutput);
}

TEST(firstLabTests, aQuedaTest) {
    std::vector<std::string> input = {
        "A QUEDA:",
        "E venha ver os deslizes que eu vou cometer;",
        "E venha ver os amigos que eu vou perder;",
        "Não tô cobrando entrada, vem ver o show na faixa.",
        "Hoje tem open bar pra ver minha desgraça."
    };

    std::vector<std::string> expectedOutput = {
        "ERROR with string: A QUEDA:"
    };

    testingProgram(input, expectedOutput);
}

TEST(firstLabTests, anotherTest) {
    std::vector<std::string> input = {
        "But I set fire to the rain.",
        "Watched it pour as- I touched your- face-",
        "Well, it burned while I cried!!!!!!!!",
        "Cause I heard it screamin' out your name;",
        "Your name."
    };

    std::vector<std::string> expectedOutput = {
        "ERROR with string: Watched it pour as- I touched your- face-",
        "ERROR with string: Well, it burned while I cried!!!!!!!!"
    };

    testingProgram(input, expectedOutput);
}

int main(int argc, char *argv[]) {
    std::cout << getenv("PATH_TO_CHILD") << std::endl;
    
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}