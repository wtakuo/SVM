/*
 * tak benchmark in C
 * by Takuo Watanabe
 */

#include <stdio.h>
#include <stdlib.h>

int tak (int x, int y, int z) {
    if (x <= y)
        return y;
    else
        return tak(tak(x - 1, y, z),
                   tak(y - 1, z, x),
                   tak(z - 1, x, y));
}

int main (int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s x y z\n", argv[0]);
        return EXIT_FAILURE;
    }
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    int z = atoi(argv[3]);

    printf(">>> %d\n", tak(x, y, z));

    return EXIT_SUCCESS;
}
