/*
 * SVM Test 1
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
          int a = 1;
          while (n != 0) {
              a = a * n;
              n = n - 1;
          }
          return a;
      }
    */
    /* 10 */ i_bipush, 1,
    /* 12 */ i_istore, 1,     /* a = 1 */
    /* loop */
    /* 14 */ i_iload, 0,
    /* 16 */ i_ifeq, 0, 17,   /* if (n == 0) goto exit; */
    /* 19 */ i_iload, 1,
    /* 21 */ i_iload, 0,
    /* 23 */ i_imul,
    /* 24 */ i_istore, 1,     /* a = a * n; */
    /* 26 */ i_iload, 0,
    /* 28 */ i_bipush, 1,
    /* 30 */ i_isub,
    /* 31 */ i_istore, 0,     /* n = n - 1; */
    /* 33 */ i_goto, -1, -22, /* goto loop; */
    /* exit */
    /* 36 */ i_iload, 1,
    /* 38 */ i_iret           /* return a; */
};

void vmtest (int n, vm_mode_t mode) {
    int codelen = sizeof(code)/sizeof(Bcode);
    Function f = new_function(1, 1, code + 10, codelen - 10);
    gv[0] = (Value)n;
    gv[1] = (Value)f;

    vm(code, codelen, vs_base, VS_SIZE, gv, GV_SIZE, mode);

    delete_function(f);
}
