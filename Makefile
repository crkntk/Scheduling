# CXX Make variable for compiler
CXX=g++
# -std=c++11  C/C++ variant to use, e.g. C++ 2011
# -Wall       show the necessary warning files
# -g3         include information for symbolic debugger e.g. gdb
CXXFLAGS=-std=c++11 -Wall -g3 -c

# object files
OBJS = process.o log.o schedule.o main.o

# Program name
PROGRAM = schedule

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
$(PROGRAM) : $(OBJS)
	$(CXX) -o $(PROGRAM) $^

process.o : process.h process.cpp
	$(CXX) $(CXXFLAGS) process.cpp
log.o : log.h log.cpp
	$(CXX) $(CXXFLAGS) log.cpp
schedule.o : schedule.h schedule.cpp
	$(CXX) $(CXXFLAGS) schedule.cpp

main.o : schedule.h main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

clean :
	rm -f *.o $(PROGRAM)
