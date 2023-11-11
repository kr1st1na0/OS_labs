#include "parent.hpp"

int main() {
    ParentProcess(getenv("PATH_TO_CHILD"));
    // bash: export PATH_TO_CHILD="/home/kristinab/ubuntu_main/OS_labs/build/lab3/child3"
    // bash: printenv PATH_TO_CHILD
    exit(EXIT_SUCCESS);
}