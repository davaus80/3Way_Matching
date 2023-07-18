# ********************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -g -std=c++11 -Wall
BIN=run_test

# ********************************************************
# Targets to bring the executable up to date

SRC=$(wildcard *.cpp)
OBJ=$(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(BIN) $^

%.o: %.c
	$(CXX) $(CXXFLAGS) $@ -c $<

clean:
	rm -f *.o
	rm $(BIN)

# testfile: Testing.o Patient.o Matcher.o
# 	$(CC) Testing.o Patient.o Matcher.o -o Testing

# Testing.o: testing.cpp Patient.h Matcher.h
# 	$(CC) testing.cpp $(CFLAGS) -o Testing.o

# Patient.o: Patient.h
# 	$(CC) patient.cppb$(CFLAGS) -o Patient.o

# Matcher.o: Matcher.h Patient.h
# 	$(CC) matcher.cpp $(CFLAGS) -o Matcher.o