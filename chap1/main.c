/* This file is intentionally empty.  You should fill it in with your
 *
 *
   solution to the programming exercise. */
#include "util.h"
#include "slp.h"
#include "prog1.h"
#include <stdio.h>

int main(int argc, char* argv[]) {
    A_stm a = prog();
    printf("%d\n",maxargs(a));
    interp(a);

}

