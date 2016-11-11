main: main.cpp SimpleTimer.o Simulation.o
	g++ -pthread -Wall -std=c++0x -o Sim02 main.cpp SimpleTimer.o Simulation.o

SimpleTimer.o: SimpleTimer.h SimpleTimer.cpp
	g++ -c SimpleTimer.cpp SimpleTimer.h

Simulation.o: Simulation.h Simulation.cpp PCB.h
	g++ -c -std=c++0x Simulation.cpp Simulation.h PCB.h

