CXX = g++ -pthread
GFLAGS = -g -std=c++11
OBJECTS = driver.o mailbox.o threadgraph.o messagepacket.o timeinterval.o
FILES = driver.cpp src/mailbox.cc src/ThreadGraph.cpp src/MessagePacket.cpp src/TimeInterval.cpp

driver: driver.o mailbox.o
	$(CXX) $(GFLAGS) -o driver $(OBJECTS)

driver.o: driver.cpp threadgraph.o messagepacket.o mailbox.o
	$(CXX) $(GFLAGS) -c driver.cpp

threadgraph.o: src/ThreadGraph.h src/ThreadGraph.cpp
	$(CXX) $(GFLAGS) -c src/ThreadGraph.cpp

messagepacket.o: src/MessagePacket.h src/MessagePacket.cpp timeinterval.o
	$(CXX) $(GFLAGS) -c src/MessagePacket.cpp

timeinterval.o: src/TimeInterval.h src/TimeInterval.cpp
	$(CXX) $(GFLAGS) -c src/TimeInterval.cpp

mailbox.o: src/mbox.h src/mailbox.cc messagepacket.o
	$(CXX) $(GFLAGS) -c src/mailbox.cc

clean:
	rm -f driver
	rm *.o

run: driver
	./driver

valgrind: driver
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./driver
