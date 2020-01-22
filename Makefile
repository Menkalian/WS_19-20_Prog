#/bin/bash

# Set configuration for compiling
CXX := /usr/bin/gcc
CXXFLAGS := -lm -Wall -pedantic-errors -o
OUTPUTName := WS_22-03-2020
SOURCEFiles := *c

# Compile program
$(OUTPUTName): $(SOURCEFiles)
	$(CXX) $(CXXFLAGS) $(OUTPUTName) $(SOURCEFiles)

# Run program
run:
	./$(OUTPUTName)
