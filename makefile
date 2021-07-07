
all: a.out

bigint.o: bigint.cpp bigint.h makefile
	clang++ -std=c++11 -w -g bigint.cpp -c -o bigint.o

a.out: main.cpp bigint.o makefile
	clang++ -std=c++11 -w -g main.cpp bigint.o
	
run: a.out
	./a.out

debug: a.out
	lldb a.out

