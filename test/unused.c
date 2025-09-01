#define NSL_UNUSED_STRIP_PREFIX
#include "unused.h"

int main(int argc, char **argv) {
    // ordinarily this would cause errors because of unused variables
    UNUSED(argc);
    UNUSED(argv);
}
