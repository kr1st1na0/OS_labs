#include "parent.hpp"

int main() {
    parentProcess(getenv("PATH_TO_CHILD"));
    // bash: export PATH_TO_CHILD="/home/kristinab/ubuntu_main/OS_labs/build/lab1/child"
    exit(EXIT_SUCCESS);
}