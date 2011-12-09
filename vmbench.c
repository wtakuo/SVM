/*
 * SVM Benchmarking
 * by Takuo Watanabe
 */

#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

#define VS_SIZE 1024
#define GV_SIZE 8

static Value vs[VS_SIZE];
static Value gv[GV_SIZE];
static Value *vs_base = vs + VS_SIZE;

static Bcode code[] = {
    /*  0 */ i_giload, 0, 1,  /* x */
    /*  3 */ i_giload, 0, 2,  /* y */
    /*  6 */ i_giload, 0, 3,  /* z */
    /*  9 */ i_gaload, 0, 0,  /* tak */
    /* 12 */ i_icall, 3,
    /* 14 */ i_showtop,
    /* 15 */ i_halt,

    /*
      int tak (int x, int y, int z) {
          if (x <= y)
              return y;
          else
              return tak(tak(x - 1, y, z),
                         tak(y - 1, z, x),
                         tak(z - 1, x, y));
      }
    */
    /* 16 */ i_iload, 0,      /* x */
    /* 18 */ i_iload, 1,      /* y */
    /* 20 */ i_isub,
    /* 21 */ i_ifle, 0, 47,   /* x <= y ? */
    /* 24 */ i_iload, 0,  
    /* 26 */ i_bipush, 1,
    /* 28 */ i_isub,
    /* 29 */ i_iload, 1,
    /* 31 */ i_iload, 2,
    /* 33 */ i_gaload, 0, 0,
    /* 36 */ i_icall, 3,
    /* 38 */ i_iload, 1,
    /* 40 */ i_bipush, 1,
    /* 42 */ i_isub,
    /* 43 */ i_iload, 2,
    /* 45 */ i_iload, 0,
    /* 47 */ i_gaload, 0, 0,
    /* 50 */ i_icall, 3,
    /* 52 */ i_iload, 2,
    /* 54 */ i_bipush, 1,
    /* 56 */ i_isub,
    /* 57 */ i_iload, 0,
    /* 59 */ i_iload, 1,
    /* 61 */ i_gaload, 0, 0,
    /* 64 */ i_icall, 3,
    /* 66 */ i_gaload, 0, 0,
    /* 69 */ i_icall, 3,
    /* 71 */ i_iload, 1,
    /* 73 */ i_iret
};

void vmtest (int x, int y, int z, vm_mode_t mode) {
    int codelen = sizeof(code)/sizeof(Bcode);
    Function f = new_function(3, 0, code + 16, codelen - 16);
    gv[0] = (Value)f;
    gv[1] = (Value)x;
    gv[2] = (Value)y;
    gv[3] = (Value)z;

    vm(code, codelen, vs_base, VS_SIZE, gv, GV_SIZE, mode);

    delete_function(f);
}

int main (int argc, char *argv[]) {
    vm_mode_t mode = vm_normal_mode;
    int x, y, z;
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
                fprintf(stderr, "usage: %s [-t][-s] x y z\n", argv[0]);
                return EXIT_FAILURE;
            }
            j++;
        }
        i++;
    }
    if (argc < i + 3) {
        fprintf(stderr, "usage: %s [-t][-s] x y z\n", argv[0]);
        return EXIT_FAILURE;
    }
    x = atoi(argv[i]);
    y = atoi(argv[i+1]);
    z = atoi(argv[i+2]);

    vmtest(x, y, z, mode);

    return EXIT_SUCCESS;
}
