CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic

SRCDIR = src
OBJS = $(SRCDIR)/main.o $(SRCDIR)/implement.o

scheduler: $(OBJS)
	$(CXX) $(CXXFLAGS) -o scheduler $(OBJS)

$(SRCDIR)/main.o: $(SRCDIR)/main.cpp $(SRCDIR)/header.h
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/main.cpp -o $(SRCDIR)/main.o

$(SRCDIR)/implement.o: $(SRCDIR)/implement.cpp $(SRCDIR)/header.h
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)/implement.cpp -o $(SRCDIR)/implement.o

clean:
	rm -f $(OBJS) scheduler

.PHONY: clean
