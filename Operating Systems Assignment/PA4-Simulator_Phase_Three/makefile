all: ConfigurationSettings.cpp Logger.cpp OperatingSystem.cpp PhaseThree.cpp ProcessControlBlock.cpp SimpleTimer.cpp InterruptSystem.cpp
	cd re2-master && make
	mv re2-master/obj/so/libre2.so.0 libre2.so.0
	g++ -std=c++11 -Wall -g ConfigurationSettings.cpp Logger.cpp OperatingSystem.cpp PhaseThree.cpp ProcessControlBlock.cpp SimpleTimer.cpp InterruptSystem.cpp -o sim03 -Wl,-rpath,'$ORIGIN' -l:libre2.so.0