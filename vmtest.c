/*
 * SVM Test
 * by Takuo Watanabe
 */

#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

void vmtest (int, vm_mode_t);

int main (int argc, char *argv[]) {
    vm_mode_t mode = vm_normal_mode;
    int n = 0;
    int i = 1;

    while (i < argc && argv[i][0] == '-') {
        char *arg = argv[i];
        int j = 1;
        while (arg[j] != '\0') {
            switch (arg[j]) {
            case 't':
                mode = vm_trace_mode;
                break;
            case 's':
                mode = vm_step_mode;
                break;
            default:
                fprintf(stderr, "usage: %s [-t][-s] [n]\n", argv[0]);
                return EXIT_FAILURE;
            }
            j++;
        }
        i++;
    }
    if (i < argc) {
        n = atoi(argv[i]);
    }

    vmtest(n, mode);

    return EXIT_SUCCESS;
}
