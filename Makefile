# -g    -> compile with debug information
# -Wall -> give verbose compiler warnings
FLAGS = -g -Wall 
COMPILE = g++

SOURCES= src/main.cpp

#Targets
all: 
	mkdir -p bin
	$(COMPILE) $(FLAGS) $(SOURCES) -o bin/a.out

clean: 
	rm bin/a.out
	rm -rf bin

