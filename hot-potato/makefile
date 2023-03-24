CXX = g++ -pthread
GFLAGS = -g -std=c++11
OBJECTS = driver.o mailbox.o threadgraph.o
FILES = driver.cpp src/mailbox.cc src/ThreadGraph.cpp

driver: driver.o mailbox.o
	$(CXX) $(GFLAGS) -o driver $(OBJECTS)

driver.o: driver.cpp threadgraph.o mailbox.o
	$(CXX) $(GFLAGS) -c driver.cpp

threadgraph.o: src/ThreadGraph.h src/ThreadGraph.cpp
	$(CXX) $(GFLAGS) -c src/ThreadGraph.cpp

mailbox.o: src/mbox.h src/mailbox.cc
	$(CXX) $(GFLAGS) -c src/mailbox.cc

clean:
	rm -f driver
	rm *.o

run: driver
	./driver

valgrind: driver
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./driver
