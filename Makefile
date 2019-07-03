CC=gcc
CCFLAGS=-Wall -g
LIBS=-ldl -pthread

.PHONY:
all: demo

.PHONY:
clean:
	-rm -f demo *.o *~ .*~

.PHONY:
format:
	clang-format -style=Google -i *.h *.c

demo: demo.o main.o sqlite3.c
	$(CC) $(CCFLAGS) -o demo $^ $(LIBS)

.c.o:
	$(CC) $(CCFLAGS) -c $<

