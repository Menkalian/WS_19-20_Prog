#/bin/bash

# Set configuration for compiling
CXX := /usr/bin/gcc
CXXFLAGS := -lm -Wall -pedantic-errors -o
OUTPUTName := WS_19-20_Prog
SOURCEFiles := *c

# Compile program
$(OUTPUTName): $(SOURCEFiles)
	$(CXX) $(CXXFLAGS) $(OUTPUTName) $(SOURCEFiles)

# Run program
run:
	./$(OUTPUTName)
