CXX = g++ -pthread
GFLAGS = -g -std=c++11
OBJECTS = driver.o mailbox.o ThreadGraph.o ThreadNode.o MessagePacket.o TimeInterval.o
FILES = driver.cpp src/mailbox.cc src/ThreadGraph.cpp src/ThreadNode.cpp src/MessagePacket.cpp src/TimeInterval.cpp

driver: driver.o mailbox.o
	$(CXX) $(GFLAGS) -o driver $(OBJECTS)

driver.o: driver.cpp threadgraph.o threadnode.o messagepacket.o mailbox.o
	$(CXX) $(GFLAGS) -c driver.cpp

ThreadGraph.o: src/ThreadGraph.h src/ThreadGraph.cpp
	$(CXX) $(GFLAGS) -c src/ThreadGraph.cpp

ThreadNode.o: src/ThreadNode.h src/ThreadNode.cpp
	$(CXX) $(GFLAGS) -c src/ThreadNode.cpp

MessagePacket.o: src/MessagePacket.h src/MessagePacket.cpp timeinterval.o
	$(CXX) $(GFLAGS) -c src/MessagePacket.cpp

TimeInterval.o: src/TimeInterval.h src/TimeInterval.cpp
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
