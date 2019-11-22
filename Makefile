all:
	ERROR: Try 'make proc' or 'make thread'
multitest.o: multitest.c
	gcc -c multitest.c
proc: searchtest.c multitest_proc.o multitest.o
	gcc -lm -o test searchtest.c multitest_proc.o multitest.o -DPROC
multitest_proc.o: multitest_proc.c multitest.o
	gcc -c multitest_proc.c 
thread: searchtest.c multitest_thread.o multitest.o
	gcc -lm -lpthread -o test searchtest.c multitest_thread.o multitest.o -DTHREAD
multitest_thread.o: multitest_thread.c multitest.o
	gcc -c multitest_thread.c
clean:
	rm test; rm *.o
