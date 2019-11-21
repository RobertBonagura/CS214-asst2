all: main.c helper.o
	gcc -o test main.c helper.o
helper.o: helper.c
	gcc -c helper.c
proc: main.c proc.o helper.o
	gcc -o test main.c proc.o helper.o
proc.o: proc.c helper.o
	gcc -c proc.c 
thread: main.c thread.o helper.o
	gcc -lpthread -o test main.c thread.o helper.o
thread.o: thread.c helper.o
	gcc -c thread.c
clean:
	rm test; rm *.o
