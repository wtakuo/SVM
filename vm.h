/*
 * SVM Definitions
 * by Takuo Watanabe
 */

typedef unsigned char Bcode;
typedef unsigned long Value;

enum {
    i_nop = 0,
    i_dup,
    i_pop,
    i_goto,
    i_ifeq,
    i_ifne,
    i_iflt,
    i_ifle,
    i_ifgt,
    i_ifge,
    i_giload,
    i_gaload,
    i_gistore,
    i_gastore,
    i_iload,
    i_aload,
    i_iload_0,
    i_aload_0,
    i_iload_1,
    i_aload_1,
    i_iload_2,
    i_aload_2,
    i_iload_3,
    i_aload_3,
    i_istore,
    i_astore,
    i_istore_0,
    i_astore_0,
    i_istore_1,
    i_astore_1,
    i_istore_2,
    i_astore_2,
    i_istore_3,
    i_astore_3,
    i_bipush,
    i_sipush,
    i_iconst_m1,
    i_iconst_0,
    i_iconst_1,
    i_iconst_2,
    i_iadd,
    i_isub,
    i_imul,
    i_idiv,
    i_irem,
    i_icall,
    i_acall,
    i_ticall,
    i_tacall,
    i_sicall,
    i_sacall,
    i_iret,
    i_aret,
    i_halt,
    i_showtop
};

typedef enum {
    vm_normal_mode,
    vm_trace_mode,
    vm_step_mode
} vm_mode_t;

#define B1INT(x) ((long)(x))
#define B2INT(x,y) (long)((signed char)(x) << 8 | (y))

void vm (Bcode code[], size_t codelen,
         Value *vs_base, size_t vs_size,
         Value gv[], size_t gv_size,
         vm_mode_t mode);

typedef struct _Function {
    size_t nargs;    /* # of arguments */
    size_t nlv;      /* # of local variables (except for args) */
    Bcode *code;
    size_t codelen;  
} *Function;

Function new_function (size_t nargs, size_t nlv, Bcode code[], size_t codelen);

void delete_function (Function f);

void dump_vm (long nsteps, Bcode *ip, Bcode code[],
              Value *lv, Value *fp, Value *sp, Value *vs_base,
              Value gv[], size_t gv_size, vm_mode_t mode);
