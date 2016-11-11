main.out: eights.o string.o
	g++ -o main.out main.cpp eights.o string.o

eights.o: eights.cpp eights.h
	g++ -c eights.cpp

string.o: string.cpp string.h
	g++ -c string.cpp
