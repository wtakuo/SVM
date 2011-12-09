# SVM Makefile 

TESTARGS = 8
BENCHARGS = 13 6 0

SRCS = svm.c tvm.c fun.c vmdebug.c \
       test1.c test2.c test3.c vmtest.c vmbench.c cbench.c
OBJS = $(subst .c,.o,$(SRCS)) svm_bench.o tvm_bench.o fun_bench.o
TEST_EXES = svmtest1 svmtest2 svmtest3 tvmtest1 tvmtest2 tvmtest3
BENCH_EXES = svmbench tvmbench cbench

CC = gcc
CFLAGS = -std=c99 -Wall -g
TVM_CFLAGS = -std=gnu99 -Wall
BENCH_FLAGS = -O3 -DDISABLE_EXECUTION_TRACE
TIME = time
RM = rm -f

all: $(TEST_EXES) $(BENCH_EXES)

test: svmtest tvmtest

svmtest: $(TEST_EXES)
	./svmtest1 $(TESTARGS)
	./svmtest2 $(TESTARGS)
	./svmtest3 $(TESTARGS)

tvmtest: $(TEST_EXES)
	./tvmtest1 $(TESTARGS)
	./tvmtest2 $(TESTARGS)
	./tvmtest3 $(TESTARGS)

bench: $(BENCH_EXES)
	$(TIME) ./svmbench $(BENCHARGS)
	$(TIME) ./svmbench $(BENCHARGS)
	$(TIME) ./tvmbench $(BENCHARGS)
	$(TIME) ./tvmbench $(BENCHARGS)
	$(TIME) ./cbench $(BENCHARGS)
	$(TIME) ./cbench $(BENCHARGS)

svmtest1: fun.o vmdebug.o vmtest.o svm.o test1.o
	$(CC) $(CFLAGS) -o $@ $^

svmtest2: fun.o vmdebug.o vmtest.o svm.o test2.o
	$(CC) $(CFLAGS) -o $@ $^

svmtest3: fun.o vmdebug.o vmtest.o svm.o test3.o
	$(CC) $(CFLAGS) -o $@ $^

tvmtest1: fun.o vmdebug.o vmtest.o tvm.o test1.o
	$(CC) $(CFLAGS) -o $@ $^

tvmtest2: fun.o vmdebug.o vmtest.o tvm.o test2.o
	$(CC) $(CFLAGS) -o $@ $^

tvmtest3: fun.o vmdebug.o vmtest.o tvm.o test3.o
	$(CC) $(CFLAGS) -o $@ $^

svmbench: fun_bench.o svm_bench.o vmbench.o
	$(CC) $(CFLAGS) $(BENCH_FLAGS) -o $@ $^

tvmbench: fun_bench.o tvm_bench.o vmbench.o
	$(CC) $(TVM_CFLAGS) $(BENCH_FLAGS) -o $@ $^

cbench: cbench.c
	$(CC) $(CFLAGS) $(BENCH_FLAGS) -o $@ $^

svm.o: svm.c vm.h
	$(CC) $(CFLAGS) -c $<

svm_bench.o: svm.c vm.h
	$(CC) $(CFLAGS) $(BENCH_FLAGS) -c -o $@ $<

tvm.o: tvm.c vm.h
	$(CC) $(TVM_CFLAGS) -c $<

tvm_bench.o: tvm.c vm.h
	$(CC) $(TVM_CFLAGS) $(BENCH_FLAGS) -c -o $@ $<

fun.o: fun.c vm.h
	$(CC) $(CFLAGS) -c $<

fun_bench.o: fun.c vm.h
	$(CC) $(CFLAGS) $(BENCH_FLAGS) -c -o $@ $<

vmdebug.o: vmdebug.c vm.h
	$(CC) $(CFLAGS) -c $<

vmtest.o: vmtest.c vm.h
	$(CC) $(CFLAGS) -c $<

test1.o: test1.c vm.h
	$(CC) $(CFLAGS) -c $<

test2.o: test2.c vm.h
	$(CC) $(CFLAGS) -c $<

test3.o: test3.c vm.h
	$(CC) $(CFLAGS) -c $<

vmbench.o: vmbench.c vm.h
	$(CC) $(CFLAGS) $(BENCH_FLAGS) -c $<

clean:
	rm -f $(TEST_EXES) $(BENCH_EXES)
	rm -f $(OBJS)
	rm -f *~ *.exe
