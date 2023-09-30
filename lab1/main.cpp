#include "parent.hpp"

int main() {
    parentProcess(getenv("PATH_TO_CHILD"));
    // bash: export PATH_TO_CHILD="../lab1/child"
    exit(EXIT_SUCCESS);
}