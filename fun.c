/*
 * SVM Function Object
 * by Takuo Watanabe
 */

#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

Function new_function (size_t nargs, size_t nlv, Bcode code[], size_t codelen) {
    Function f = malloc(sizeof(struct _Function));
    if (f == NULL) {
        perror("new_function");
        exit(EXIT_FAILURE);
    }
    f->nargs = nargs;
    f->nlv = nlv;
    f->code = code;
    f->codelen = codelen;

    return f;
}

void delete_function (Function f) {
    free(f);
}
