#include "workpiece.h"
#include <stdio.h>

int main(int argc, const char* argv[]) {
    workpiece w(argv[1]);
    printf("The cost of the workpiece is $%2.2lf\n",w.quote());
    return 0;
}


