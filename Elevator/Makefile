#This file generates the executable for the project i.e. ElevatorControlAgent.exe
CPP     = g++
BIN 	= ElevatorControlAgent

OBJECTS = main.o ./internal/eTime.o
CFLAGS=-std=c++11
INCLUDE_PATHS = -I. -I./internal -I./vendor -I./pdevslib
INCLUDE_USER_PATHS = -I./user_models

all: $(OBJECTS)
	$(CPP) -g -o $(BIN) $(OBJECTS)

main.o: main.cpp
		$(CPP) -g -c $(CFLAGS) $(INCLUDE_PATHS) $(INCLUDE_USER_PATHS) main.cpp -o main.o


internal/eTime.o:
	$(CPP) -g -c $(CFLAGS) $(INCLUDE_PATHS) $(INCLUDE_USER_PATHS) ./internal/eTime.cpp -o internal/eTime.o

clean:
	rm -f $(BIN) *.o *~
	-for d in internal; do (cd $$d; rm -f *.o *~ ); done
