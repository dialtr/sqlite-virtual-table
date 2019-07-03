CC=g++
CCFLAGS=-Wall -Werror -g
LIBS=-lsqlite3

.PHONY:
all: demo

.PHONY:
clean:
	-rm -f demo *.o *~ .*~

.PHONY:
format:
	clang-format -style=Google -i *.h *.c

demo: demo.o main.o
	$(CXX) $(CCFLAGS) -o demo $^ $(LIBS)

.c.o:
	$(CC) $(CXXFLAGS) -c $<

