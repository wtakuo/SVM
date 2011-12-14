/*
 * SVM Test 3
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
      show f(n, 1);
      halt;
    */
    /*  0 */ i_giload, 0, 0,
    /*  3 */ i_bipush, 1,
    /*  5 */ i_gaload, 0, 1,
    /*  8 */ i_icall, 2,  /* f(n, 1) */
    /* 10 */ i_showtop,
    /* 11 */ i_halt,

    /*
      int f (int n, int a) {
          if (n <= 0)
              return a;
          else
          return f(n - 1, n * a);
      }
    */
    /* 12 */ i_iload, 0,
    /* 14 */ i_ifgt, 0, 3,
    /* 17 */ i_iload, 1,
    /* 19 */ i_iret,
    /* 20 */ i_iload, 0,
    /* 22 */ i_bipush, 1,
    /* 24 */ i_isub,
    /* 25 */ i_iload, 0,
    /* 27 */ i_iload, 1,
    /* 29 */ i_imul,
    /* 30 */ i_gaload, 0, 1,
    /* 33 */ i_sicall, 2,
};

void vmtest (int n, vm_mode_t trace) {
    int codelen = sizeof(code)/sizeof(Bcode);
    Function f = new_function(2, 0, code + 12, codelen - 12);
    gv[0] = (Value)n;
    gv[1] = (Value)f;

    vm(code, codelen, vs_base, VS_SIZE, gv, GV_SIZE, trace);

    delete_function(f);
}

