typedef struct _Function {
    size_t nargs;    /* # of arguments */
    size_t nlv;      /* # of local variables (except for args) */
    Bcode *code;
    size_t codelen;  
} *Function;

Function new_function (size_t nargs, size_t nlv, Bcode code[], size_t codelen);

void delete_function (Function f);
