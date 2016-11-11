main.out: main.cpp string.o
	g++ -o main.out main.cpp string.o

string.o: string.cpp string.h
	g++ -c string.cpp
