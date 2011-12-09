/*
 * SVM Execution Tracing Code
 * by Takuo Watanabe
 */

#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

static char *instr_name (Bcode instr) {
    switch (instr) {
    case i_nop: return "nop";
    case i_dup: return "dup";
    case i_pop: return "pop";
    case i_goto: return "goto";
    case i_ifeq: return "ifeq";
    case i_ifne: return "ifne";
    case i_iflt: return "iflt";
    case i_ifle: return "ifle";
    case i_ifgt: return "ifgt";
    case i_ifge: return "ifge";
    case i_giload: return "giload";
    case i_gaload: return "gaload";
    case i_gistore: return "gistore";
    case i_gastore: return "gastore";
    case i_iload: return "iload";
    case i_aload: return "aload";
    case i_istore: return "istore";
    case i_astore: return "astore";
    case i_bipush: return "bipush";
    case i_sipush: return "sipush";
    case i_iadd: return "iadd";
    case i_isub: return "isub";
    case i_imul: return "imul";
    case i_idiv: return "idiv";
    case i_irem: return "irem";
    case i_icall: return "icall";
    case i_acall: return "acall";
    case i_ticall: return "ticall";
    case i_tacall: return "tacall";
    case i_sicall: return "sicall";
    case i_sacall: return "sacall";
    case i_iret: return "iret";
    case i_aret: return "aret";
    case i_halt: return "halt";
    case i_showtop: return "showtop";
    default: return "???";
    }
}

void dump_vm (long nsteps, Bcode *ip, Bcode code[],
              Value *lv, Value *fp, Value *sp, Value *vs_base,
              Value gv[], size_t gv_size, vm_mode_t mode) {
    long i;
    Value *p;
    if (mode == vm_normal_mode) return;

    /* global variables */
    for (i = 0; i < gv_size; i++) {
        printf("gv[%ld]=0x%08lx ", i, gv[i]);
        if ((i+1) % 4 == 0) printf("\n");
    }
    if (i % 4 != 0) printf("\n");
    /* value stack */
    i = 0;
    for (p = vs_base - 1; p >= sp; p--) {
        printf("vs[%ld]=0x%08lx ", vs_base - 1 - p, *p);
        if ((i+1) % 4 == 0) printf("\n");
        i++;
    }
    if (i % 4 != 0) printf("\n");
    /* machine state */
    printf("lv=%p (%ld), fp=%p (%ld), sp=%p (%ld)\n", 
           lv, vs_base - lv, 
           fp, vs_base - fp,
           sp, vs_base - sp);
    /* next instruction */
    printf("nsteps=%ld, ip=%p, pc=%ld, inst=%s\n",
           nsteps, 
           ip,
           ip - code, 
           instr_name(ip[0]));

    if (mode == vm_step_mode) {
        printf("next? [Y/n]");
        switch (getchar()) {
        case 'n':
        case 'N':
            exit(EXIT_SUCCESS);
        }
    }
}
