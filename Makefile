CXX=g++
CXXFLAGS=-Wall -Werror -g -std=c++14
LIBS=-lsqlite3

.PHONY:
all: demo

.PHONY:
clean:
	-rm -f demo *.o *~ .*~

.PHONY:
format:
	clang-format -style=Google -i *.h *.cc

demo: demo.o main.o
	$(CXX) $(CXXFLAGS) -o demo $^ $(LIBS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<

