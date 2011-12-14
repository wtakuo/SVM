/*
 * SVM Test 2
 * by Takuo Watanabe
 */

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "fun.h"

#define VS_SIZE 256
#define GV_SIZE 8

static Value vs[VS_SIZE];
static Value gv[GV_SIZE];
static Value *vs_base = vs + VS_SIZE;

static Bcode code[] = {
    /*
      show f(n);
      halt;
    */
    /*  0 */ i_giload, 0, 0,
    /*  3 */ i_gaload, 0, 1,
    /*  6 */ i_icall, 1,  /* f(n) */
    /*  8 */ i_showtop,
    /*  9 */ i_halt,

    /*
      int f (int n) {
          if (n <= 0)
              return 1;
          else
              return n * f(n - 1);
      }
    */
    /* 10 */ i_iload, 0,
    /* 12 */ i_ifgt, 0, 3,
    /* 15 */ i_bipush, 1,
    /* 17 */ i_iret,
    /* label */
    /* 18 */ i_iload, 0,
    /* 20 */ i_dup,
    /* 21 */ i_bipush, 1,
    /* 23 */ i_isub,
    /* 24 */ i_gaload, 0, 1,
    /* 27 */ i_icall, 1,
    /* 29 */ i_imul,
    /* 30 */ i_iret
};

void vmtest (int n, int trace) {
    int codelen = sizeof(code)/sizeof(Bcode);
    Function f = new_function(1, 0, code + 10, codelen - 10);
    gv[0] = (Value)n;
    gv[1] = (Value)f;

    vm(code, codelen, vs_base, VS_SIZE, gv, GV_SIZE, trace);

    delete_function(f);
}
