# ********************************************************
# Variables to control Makefile operation

CXX=g++
CXXFLAGS=-std=c++11 -Wall -O -g -MMD -Werror=vla
SOURCES=Patient.cc Patient_KDTree.cc Matcher.cc Testing.cc main.cc
OBJECTS=${SOURCES:.cc=.o} # .o files depend upon .cc files with the same names
DEPENDS=${OBJECTS:.o=.d}
EXEC=prop_match

# ********************************************************
# Targets to bring the executable up to date

# First target in the makefile is the default target.
# Note that the LIBFLAGS must come last in the command
$(EXEC): $(OBJECTS)
	echo $(SOURCES[0])
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

-include ${DEPENDS}

.PHONY: clean
clean:
	rm  -f $(OBJECTS) $(DEPENDS) $(EXEC)



## OLD VERSION

# test_exec: Testing.o Patient.o Matcher.o
# 	$(CXX) $(CXXFLAGS) Testing.o Patient.o Matcher.o -o test_exec

# Testing.o: testing.cpp Patient.h Matcher.h
# 	$(CXX) $(CXXFLAGS) Testing.cpp  -o Testing.o

# Matcher.o: Matcher.h Patient.h
# 	$(CXX) $(CXXFLAGS) Matcher.cpp -o Matcher.o

# Patient_KDTree.o: Patient_KDTree.h Patient.h
# 	$(CXX) $(CXXFLAGS) Patient_KDTree.cpp -o Patient_KDTree.o

# Patient.o: Patient.h
# 	$(CC) Patient.cpp $(CFLAGS) -o Patient.o