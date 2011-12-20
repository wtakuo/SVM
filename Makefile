# SVM Makefile 

TEST_ARGS = 8
BENCH_ARGS = 13 6 0

VM_SRCS = svm.c tvm.c
LIB_SRCS = fun.c vmdebug.c vmtest.c
TEST_SRCS = test1.c test2.c test3.c
BENCH_SRCS = vmbench.c cbench.c
SRCS = $(VM_SRCS) $(LIB_SRCS) $(TEST_SRCS) $(BENCH_SRCS)
OBJS = $(SRCS:%.c=%.o) svm_opt.o tvm_opt.o fun_opt.o vmbench_opt.o
DEPS = $(OBJS:%.o=%.d)

TEST_EXES = $(TEST_SRCS:test%.c=svm_test%) $(TEST_SRCS:test%.c=tvm_test%)
BENCH_EXES = svmbench tvmbench cbench

CC = gcc
CPPFLAGS =
OPTFLAGS = -O3 -DDISABLE_EXECUTION_TRACE
DEBUGFLAGS = -g
LDFLAGS =
CFLAGS = -std=gnu99 -Wall -Werror -MMD $(DEBUGFLAGS)

TIME = time -p

%_opt.o: %.c
	$(CC) $(CFLAGS) $(OPTFLAGS) -c -o $@ $<

svm_test%: svm.o fun.o vmdebug.o vmtest.o test%.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

tvm_test%: tvm.o fun.o vmdebug.o vmtest.o test%.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

.PHONY: all clean svmtest tvmtest bench

.SECONDARY: $(OBJS)

all: $(TEST_EXES) $(BENCH_EXES)

svmtest: $(TEST_EXES)
	./svm_test1 $(TEST_ARGS)
	./svm_test2 $(TEST_ARGS)
	./svm_test3 $(TEST_ARGS)

tvmtest: $(TEST_EXES)
	./tvm_test1 $(TEST_ARGS)
	./tvm_test2 $(TEST_ARGS)
	./tvm_test3 $(TEST_ARGS)

bench: $(BENCH_EXES)
	$(TIME) ./svmbench $(BENCH_ARGS)
	$(TIME) ./tvmbench $(BENCH_ARGS)
	$(TIME) ./cbench $(BENCH_ARGS)

svmbench: svm_opt.o fun_opt.o vmbench_opt.o
	$(CC) $(CFLAGS) $(OPTFLAGS) $(LDFLAGS) -o $@ $^

tvmbench: tvm_opt.o fun_opt.o vmbench_opt.o
	$(CC) $(CFLAGS) $(OPTFLAGS) $(LDFLAGS) -o $@ $^

cbench: cbench.c
	$(CC) $(CFLAGS) $(OPTFLAGS) $(LDFLAGS) -o $@ $^

-include $(DEPS)

clean:
	$(RM) $(TEST_EXES) $(BENCH_EXES)
	$(RM) $(OBJS) $(DEPS)
	$(RM) -r *.dSYM a.out *~

