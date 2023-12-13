#include <gtest/gtest.h>

#include "set"

#include "nodeRoutine.hpp"
#include "socketRoutine.hpp"

TEST(FifthSeventhLabTests, PingTest) {
    std::string programPath = getenv("PROGRAM_PATH");
    std::set<int> Nodes;
    Node task(-1);
    Nodes.insert(-1);
    task.Create(1, programPath);
    Nodes.insert(1);

    std::string ans = task.Send("ping 1", 1);
    EXPECT_EQ(ans, "Ok: 1");

    ans = task.Send("ping 2", 2);
    EXPECT_EQ(ans, "Error: Not found");

    task.Kill();
}

TEST(FifthSeventhLabTests, ExecTest) {
    std::string programPath = getenv("PROGRAM_PATH");
    std::set<int> Nodes;
    Node task(-1);
    Nodes.insert(-1);
    task.Create(1, programPath);
    Nodes.insert(1);

    std::string ans = task.Send("exec 5 6 4 2 8 5", 1);
    EXPECT_EQ(ans, "Ok: 1: 25");

    task.Kill();
}

TEST(FifthSeventhLabTests, FullTest) {
    std::string programPath = getenv("PROGRAM_PATH");
    std::string ans;
    std::set<int> Nodes;
    Node task(-1);
    Nodes.insert(-1);
    task.Create(1, programPath);
    Nodes.insert(1);
    task.Send("create 2", 1);
    Nodes.insert(2);
    task.Send("create 3", 2);
    Nodes.insert(3);
    
    ans = task.Send("ping 1", 1);
    EXPECT_EQ(ans, "Ok: 1");
    ans = task.Send("ping 2", 2);
    EXPECT_EQ(ans, "Ok: 1");

    ans = task.Send("exec 3 1 2 3", 2);
    EXPECT_EQ(ans, "Ok: 2: 6");

    ans = task.Send("exec 5 1 1 1 1 1", 3);
    EXPECT_EQ(ans, "Ok: 3: 5");

    task.Kill();
}

int main(int argc, char *argv[]) {
    std::cout << getenv("PROGRAM_PATH") << std::endl;
    // bash: export PROGRAM_PATH="/home/kristinab/ubuntu_main/OS_labs/build/lab5-7/server"
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}