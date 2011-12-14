/*
 * A Small Virtual Machine using Direct Threaded Code (for GCC)
 * by Takuo Watanabe
 */

#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "fun.h"

#ifdef DISABLE_EXECUTION_TRACE
#define NEXT goto **ip
#else
#define NEXT dump_vm(++nsteps, code + (ip - tcode), code,       \
                     lv, fp, sp, vs_base, gv, gv_size, mode);	\
    goto **ip
#endif

typedef void *Tcode;

static void b2t (Bcode code[], Tcode tcode[], size_t codelen, 
                 const Tcode labels[]) {
    int i = 0;
    while (i < codelen) {
        tcode[i] = labels[code[i]];
        switch (code[i]) {
        case i_goto:
        case i_ifeq:
        case i_ifne:
        case i_iflt:
        case i_ifle:
        case i_ifgt:
        case i_ifge:
            tcode[i + 1] = (Tcode)B2INT(code[i + 1], code[i + 2]);
            tcode[i + 2] = NULL;
            i += 2;
            break;
        case i_giload:
        case i_gaload:
        case i_gistore:
        case i_gastore:
            /* tcode[i + 1] = (Tcode)((int)code[i+1] << 8 | (int)code[i+2]); */
            tcode[i + 1] = (Tcode)B2INT(code[i + 1], code[i + 2]);
            tcode[i + 2] = NULL;
            i += 2;
            break;
        case i_iload:
        case i_aload:
        case i_istore:
        case i_astore:
        case i_bipush:
        case i_icall:
        case i_acall:
        case i_ticall:
        case i_tacall:
        case i_sicall:
        case i_sacall:
            /* tcode[i+1] = (Tcode)((int)code[i+1]); */
            tcode[i + 1] = (Tcode)B1INT(code[i + 1]);
            i++;
            break;
        case i_sipush:
            tcode[i + 1] = (Tcode)((long)code[1] + (long)code[2] * 256);
            tcode[i + 2] = NULL;
            i += 2;
            break;
        }
        i++;
    }
}

void vm (Bcode code[], size_t codelen,
         Value *vs_base, size_t vs_size,
         Value gv[], size_t gv_size,
         vm_mode_t mode) {
    static const Tcode labels[] = {
        &&l_nop,
        &&l_dup,
        &&l_pop,
        &&l_goto,
        &&l_ifeq,
        &&l_ifne,
        &&l_iflt,
        &&l_ifle,
        &&l_ifgt,
        &&l_ifge,
        &&l_giload,
        &&l_gaload,
        &&l_gistore,
        &&l_gastore,
        &&l_iload,
        &&l_aload,
        &&l_iload_0,
        &&l_aload_0,
        &&l_iload_1,
        &&l_aload_1,
        &&l_iload_2,
        &&l_aload_2,
        &&l_iload_3,
        &&l_aload_3,
        &&l_istore,
        &&l_astore,
        &&l_istore_0,
        &&l_astore_0,
        &&l_istore_1,
        &&l_astore_1,
        &&l_istore_2,
        &&l_astore_2,
        &&l_istore_3,
        &&l_astore_3,
        &&l_bipush,
        &&l_sipush,
        &&l_iconst_m1,
        &&l_iconst_0,
        &&l_iconst_1,
        &&l_iconst_2,
        &&l_iadd,
        &&l_isub,
        &&l_imul,
        &&l_idiv,
        &&l_irem,
        &&l_icall,
        &&l_acall,
        &&l_ticall,
        &&l_tacall,
        &&l_sicall,
        &&l_sacall,
        &&l_iret,
        &&l_aret,
        &&l_halt,
        &&l_showtop
    };
    Tcode tcode[codelen];

    /* machine registers */
    Tcode *ip;          /* instruction pointer */
    Value *sp = vs_base;  /* stack pointer */
    Value *fp = sp;       /* frame pointer */
    Value *lv = sp;       /* local variable base pointer */
#ifndef DISABLE_EXECUTION_TRACE
    long nsteps = 0;
#endif

    Function f;
    int nargs;
    Value ip_save, lv_save, fp_save;
    Value *arg;
    Value *newfp;

    b2t(code, tcode, codelen, labels);

    /* start VM */
    ip = tcode;
    NEXT;

    return;

 l_nop:
    ip++;
    NEXT;
 l_dup:
    sp--;
    sp[0] = sp[1];
    ip++;
    NEXT;
 l_pop:
    sp++;
    ip++;
    NEXT;
 l_goto:
    ip += (long)ip[1] + 3;
    NEXT;
 l_ifeq:
    if ((int)sp[0] == 0)
        ip += (long)ip[1];
    ip += 3;
    sp++;
    NEXT;
 l_ifne:
    if ((int)sp[0] != 0)
        ip += (long)ip[1];
    ip += 3;
    sp++;
    NEXT;
 l_iflt:
    if ((int)sp[0] < 0)
        ip += (long)ip[1];
    ip += 3;
    sp++;
    NEXT;
 l_ifle:
    if ((int)sp[0] <= 0)
        ip += (long)ip[1];
    ip += 3;
    sp++;
    NEXT;
 l_ifgt:
    if ((int)sp[0] > 0)
        ip += (long)ip[1];
    ip += 3;
    sp++;
    NEXT;
 l_ifge:
    if ((int)sp[0] >= 0)
        ip += (long)ip[1];
    ip += 3;
    sp++;
    NEXT;
 l_giload:
 l_gaload:
    sp--;
    sp[0] = gv[(long)ip[1]];
    ip += 3;
    NEXT;
 l_gistore:
 l_gastore:
    gv[(long)ip[1]] = sp[0];
    sp++;
    ip += 3;
    NEXT;
 l_iload:
 l_aload:
    sp--;
    sp[0] = lv[-(long)ip[1]];
    ip += 2;
    NEXT;
 l_iload_0:
 l_aload_0:
    sp--;
    sp[0] = lv[0];
    ip += 1;
    NEXT;
 l_iload_1:
 l_aload_1:
    sp--;
    sp[0] = lv[-1];
    ip += 1;
    NEXT;
 l_iload_2:
 l_aload_2:
    sp--;
    sp[0] = lv[-2];
    ip += 1;
    NEXT;
 l_iload_3:
 l_aload_3:
    sp--;
    sp[0] = lv[-3];
    ip += 1;
    NEXT;
 l_istore:
 l_astore:
    lv[-(long)ip[1]] = sp[0];
    sp++;
    ip += 2;
    NEXT;
 l_istore_0:
 l_astore_0:
    lv[0] = sp[0];
    sp++;
    ip += 1;
    NEXT;
 l_istore_1:
 l_astore_1:
    lv[-1] = sp[0];
    sp++;
    ip += 1;
    NEXT;
 l_istore_2:
 l_astore_2:
    lv[-2] = sp[0];
    sp++;
    ip += 1;
    NEXT;
 l_istore_3:
 l_astore_3:
    lv[-3] = sp[0];
    sp++;
    ip += 1;
    NEXT;
 l_bipush:
    sp--;
    sp[0] = (Value)ip[1];
    ip += 2;
    NEXT;
 l_sipush:
    sp--;
    sp[0] = (Value)ip[1];
    ip += 3;
    NEXT;
 l_iconst_m1:
    sp--;
    sp[0] = (Value)-1;
    ip += 1;
    NEXT;
 l_iconst_0:
    sp--;
    sp[0] = (Value)0;
    ip += 1;
    NEXT;
 l_iconst_1:
    sp--;
    sp[0] = (Value)1;
    ip += 1;
    NEXT;
 l_iconst_2:
    sp--;
    sp[0] = (Value)2;
    ip += 1;
    NEXT;
 l_iadd:
    sp[1] = sp[1] + sp[0];
    sp++;
    ip++;
    NEXT;
 l_isub:
    sp[1] = sp[1] - sp[0];
    sp++;
    ip++;
    NEXT;
 l_imul:
    sp[1] = sp[1] * sp[0];
    sp++;
    ip++;
    NEXT;
 l_idiv:
    sp[1] = sp[1] / sp[0];
    sp++;
    ip++;
    NEXT;
 l_irem:
    sp[1] = sp[1] % sp[0];
    sp++;
    ip++;
    NEXT;
 l_icall:
 l_acall:
    f = (Function)sp[0];
    nargs = (long)ip[1];
    newfp = sp - f->nlv - 2;
    newfp[0] = (Value)(ip + 2);
    newfp[1] = (Value)lv;
    newfp[2] = (Value)fp;
    lv = sp + nargs;
    sp = fp = newfp;
    ip = tcode + (f->code - code);
    NEXT;
 l_ticall:
 l_tacall:
    f = (Function)sp[0];
    nargs = (long)ip[1];
    ip_save = fp[0];
    lv_save = fp[1];
    fp_save = fp[2];
    arg = sp + nargs;
    for (int i = 0; i < nargs ; i++)
        lv[-i] = arg[-i];
    sp = fp = lv - nargs - f->nlv - 3;
    fp[0] = ip_save;
    fp[1] = lv_save;
    fp[2] = fp_save;
    ip = tcode + (f->code - code);
    NEXT;
 l_sicall:
 l_sacall:
    f = (Function)sp[0];
    nargs = (long)ip[1];
    arg = sp + nargs;
    for (int i = 0; i < nargs; i++)
        lv[-i] = arg[-i];
    sp = fp;
    ip = tcode + (f->code - code);
    NEXT;
 l_iret:
 l_aret:
    lv[0] = sp[0];
    sp = lv;
    ip = (Tcode)fp[0];
    lv = (Value *)fp[1];
    fp = (Value *)fp[2];
    NEXT;
 l_halt:
    return;
 l_showtop:
    printf(">>> %d\n", (int)sp[0]);
    sp++;
    ip++;
    NEXT;
}
