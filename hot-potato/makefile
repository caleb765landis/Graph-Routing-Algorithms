CXX = g++ -pthread
GFLAGS = -g -std=c++11
OBJECTS = driver.o mailbox/mailbox.o threadgraph.o
FILES = driver.cpp mailbox.cc ThreadGraph.cpp

driver: driver.o mailbox/mailbox.o
	$(CXX) $(GFLAGS) -o driver $(OBJECTS)

driver.o: driver.cpp threadgraph.o
	$(CXX) $(GFLAGS) -c driver.cpp

threadgraph.o: ThreadGraph.h ThreadGraph.cpp
	$(CXX) $(GFLAGS) -c ThreadGraph.cpp

clean:
	rm -f driver
	rm *.o

run: driver
	./driver

valgrind: driver
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./driver
