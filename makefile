all: szw 
OBJ = equation_puzzle.o shuttle_bus.o test_lines.o safe_challenge.o machine_learning.o
#CPPFLAGS = -O -Wall -lpthread
CPPFLAGS = -O -w -lpthread
szw: $(OBJ)
	g++ -o szw machine_learning.o $(CPPFLAGS)
.cpp.o:
	g++ -c $<
.PHONY: clean cleanall
clean:
	-rm szw *.o
cleanall:
	-make clean 

