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

TEST_EXES = $(TEST_SRCS:test%.c=svmtest%) $(TEST_SRCS:test%.c=tvmtest%)
BENCH_EXES = svmbench tvmbench cbench

CC = gcc
CPPFLAGS =
OPTFLAGS = -O3 -DDISABLE_EXECUTION_TRACE
DEBUGFLAGS = -g
LDFLAGS =
CFLAGS = -std=gnu99 -Wall -Werror -MMD $(DEBUGFLAGS)

TIME = time -p

.PHONY: all clean svmtest tvmtest bench

all: $(TEST_EXES) $(BENCH_EXES)

svmtest: $(TEST_EXES)
	./svmtest1 $(TEST_ARGS)
	./svmtest2 $(TEST_ARGS)
	./svmtest3 $(TEST_ARGS)

tvmtest: $(TEST_EXES)
	./tvmtest1 $(TEST_ARGS)
	./tvmtest2 $(TEST_ARGS)
	./tvmtest3 $(TEST_ARGS)

bench: $(BENCH_EXES)
	$(TIME) ./svmbench $(BENCH_ARGS)
	$(TIME) ./tvmbench $(BENCH_ARGS)
	$(TIME) ./cbench $(BENCH_ARGS)

svmtest%: svm.o fun.o vmdebug.o vmtest.o test%.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

tvmtest%: tvm.o fun.o vmdebug.o vmtest.o test%.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%_opt.o: %.c
	$(CC) $(CFLAGS) $(OPTFLAGS) -c -o $@ $<

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

