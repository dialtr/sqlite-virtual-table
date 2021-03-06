CC=gcc
CCFLAGS=-Wall -g
LIBS=-ldl -pthread -lsqlite3

.PHONY:
all: demo

.PHONY:
clean:
	-rm -f demo *.o *~ .*~

.PHONY:
format:
	clang-format -style=Google -i *.h *.c

demo: demo.o main.o
	$(CC) $(CCFLAGS) -o demo $^ $(LIBS)

.c.o:
	$(CC) $(CCFLAGS) -c $<

