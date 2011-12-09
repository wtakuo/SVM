/*
 * SVM: A Small Virtual Machine using Giant Switch
 * by Takuo Watanabe
 */

#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

void vm (Bcode code[], size_t codelen,
         Value *vs_base, size_t vs_size,
         Value gv[], size_t gv_size,
         vm_mode_t mode) {

    Bcode *ip = code;     // instruction pointer
    Value *sp = vs_base;  // stack pointer
    Value *fp = sp;       // frame pointer
    Value *lv = sp;       // local variable base pointer

#ifndef DISABLE_EXECUTION_TRACE
    long nsteps = 0;
#endif

    for (;;) {
#ifndef DISABLE_EXECUTION_TRACE
        nsteps++;
        dump_vm(nsteps, ip, code, lv, fp, sp, vs_base, gv, gv_size, mode);
#endif

        switch (*ip) {
        case i_nop:
            ip++;
            break;
        case i_dup:
            sp--;
            sp[0] = sp[1];
            ip++;
            break;
        case i_pop:
            sp++;
            ip++;
            break;
        case i_goto:
            ip += B2INT(ip[1], ip[2]) + 3;
            break;
        case i_ifeq:
            if ((int)sp[0] == 0)
                ip += B2INT(ip[1], ip[2]);
            ip += 3;
            sp++;
            break;
        case i_ifne:
            if ((int)sp[0] != 0)
                ip += B2INT(ip[1], ip[2]);
            ip += 3;
            sp++;
            break;
        case i_iflt:
            if ((int)sp[0] < 0)
                ip += B2INT(ip[1], ip[2]);
            ip += 3;
            sp++;
            break;
        case i_ifle:
            if ((int)sp[0] <= 0)
                ip += B2INT(ip[1], ip[2]);
            ip += 3;
            sp++;
            break;
        case i_ifgt:
            if ((int)sp[0] > 0)
                ip += B2INT(ip[1], ip[2]);
            ip += 3;
            sp++;
            break;
        case i_ifge:
            if ((int)sp[0] >= 0)
                ip += B2INT(ip[1], ip[2]);
            ip += 3;
            sp++;
            break;
        case i_giload:
        case i_gaload:
            sp--;
            /* sp[0] = gv[(int)ip[1] << 8 | (int)ip[2]]; */
            sp[0] = gv[B2INT(ip[1], ip[2])];
            ip += 3;
            break;
        case i_gistore:
        case i_gastore:
            /* gv[(int)ip[1] << 8 | (int)ip[2]] = sp[0]; */
            gv[B2INT(ip[1], ip[2])] = sp[0];
            sp++;
            ip += 3;
            break;
        case i_iload:
        case i_aload:
            sp--;
            /* sp[0] = lv[-(int)ip[1]]; */
            sp[0] = lv[-B1INT(ip[1])];
            ip += 2;
            break;
        case i_iload_0:
        case i_aload_0:
            sp--;
            sp[0] = lv[0];
            ip += 1;
            break;
        case i_iload_1:
        case i_aload_1:
            sp--;
            sp[0] = lv[-1];
            ip += 1;
            break;
        case i_iload_2:
        case i_aload_2:
            sp--;
            sp[0] = lv[-2];
            ip += 1;
            break;
        case i_iload_3:
        case i_aload_3:
            sp--;
            sp[0] = lv[-3];
            ip += 1;
            break;
        case i_istore:
        case i_astore:
            /* lv[-(int)ip[1]] = sp[0]; */
            lv[-B1INT(ip[1])] = sp[0];
            sp++;
            ip += 2;
            break;
        case i_istore_0:
        case i_astore_0:
            lv[0] = sp[0];
            sp++;
            ip += 1;
            break;
        case i_istore_1:
        case i_astore_1:
            lv[-1] = sp[0];
            sp++;
            ip += 1;
            break;
        case i_istore_2:
        case i_astore_2:
            lv[-2] = sp[0];
            sp++;
            ip += 1;
            break;
        case i_istore_3:
        case i_astore_3:
            lv[-3] = sp[0];
            sp++;
            ip += 1;
            break;
        case i_bipush:
            sp--;
            /* sp[0] = (Value)ip[1]; */
            sp[0] = (Value)B1INT(ip[1]);
            ip += 2;
            break;
        case i_sipush:
            sp--;
            sp[0] = (Value)((int)ip[1] + (int)ip[2] * 256);
            ip += 3;
            break;
        case i_iconst_m1:
            sp--;
            sp[0] = (Value)-1;
            ip++;
            break;
        case i_iconst_0:
            sp--;
            sp[0] = (Value)0;
            ip++;
            break;
        case i_iconst_1:
            sp--;
            sp[0] = (Value)1;
            ip++;
            break;
        case i_iconst_2:
            sp--;
            sp[0] = (Value)2;
            ip++;
            break;
        case i_iadd:
            sp[1] = sp[1] + sp[0];
            sp++;
            ip++;
            break;
        case i_isub:
            sp[1] = sp[1] - sp[0];
            sp++;
            ip++;
            break;
        case i_imul:
            sp[1] = sp[1] * sp[0];
            sp++;
            ip++;
            break;
        case i_idiv:
            sp[1] = sp[1] / sp[0];
            sp++;
            ip++;
            break;
        case i_irem:
            sp[1] = sp[1] % sp[0];
            sp++;
            ip++;
            break;
        case i_icall: 
        case i_acall: {
            Function f = (Function)sp[0];
            int nargs = (int)ip[1];
            Value *newfp = sp - f->nlv - 2;
            newfp[0] = (Value)(ip + 2);
            newfp[1] = (Value)lv;
            newfp[2] = (Value)fp;
            lv = sp + nargs;
            sp = fp = newfp;
            ip = f->code;
            break;
        }
        case i_ticall:
        case i_tacall: {
            Function f = (Function)sp[0];
            int nargs = (int)ip[1];
            Value ip_save = fp[0];
            Value lv_save = fp[1];
            Value fp_save = fp[2];
            Value *arg = sp + nargs;
            for (int i = 0; i < nargs ; i++)
                lv[-i] = arg[-i];
            fp = lv - nargs - f->nlv - 3;
            sp = fp;
            fp[0] = ip_save;
            fp[1] = lv_save;
            fp[2] = fp_save;
            ip = f->code;
            break;
        }
        case i_sicall:
        case i_sacall: {
            Function f = (Function)sp[0];
            int nargs = (int)ip[1];
            Value *arg = sp + nargs;
            for (int i = 0; i < nargs; i++)
                lv[-i] = arg[-i];
            sp = fp;
            ip = f->code;
            break;
        }
        case i_iret:
        case i_aret:
            lv[0] = sp[0];
            sp = lv;
            ip = (Bcode *)fp[0];
            lv = (Value *)fp[1];
            fp = (Value *)fp[2];
            break;
        case i_halt:
            return;
        case i_showtop:
            printf(">>> %d\n", (int)sp[0]);
            sp++;
            ip++;
            break;
        default:
            fprintf(stderr, "Invalid instruction: %d\n", (int)ip[-1]);
            return;
        }
    }
}
