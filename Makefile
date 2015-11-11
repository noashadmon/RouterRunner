router.out : router.o RouterRunner.o  
	g++ -ansi -Wall -pg -o router.out router.o RouterRunner.o  

router.o : router.cpp router.h RouterRunner.h 
	g++ -ansi -Wall -pg -c router.cpp

RouterRunner.o : RouterRunner.cpp CPUTimer.h RouterRunner.h router.h 
	g++ -ansi -Wall -pg -c RouterRunner.cpp

clean : 
	rm -f router.out router.o  RouterRunner.o   
