CXX = g++ -pthread
GFLAGS = -g -std=c++11
OBJECTS = driver.o mailbox.o ThreadGraph.o Node.o ThreadNode.o MessagePacket.o TimeInterval.o RandomNodes.o
FILES = driver.cpp src/mailbox.cc src/ThreadGraph.cpp src/ThreadNode.cpp src/MessagePacket.cpp src/TimeInterval.cpp src/Node.cpp src/RandomNodes.cpp

driver: driver.o mailbox.o
	$(CXX) $(GFLAGS) -o driver $(OBJECTS)

driver.o: driver.cpp ThreadGraph.o Node.o ThreadNode.o MessagePacket.o mailbox.o
	$(CXX) $(GFLAGS) -c driver.cpp

ThreadGraph.o: src/ThreadGraph.h src/ThreadGraph.cpp
	$(CXX) $(GFLAGS) -c src/ThreadGraph.cpp

ThreadNode.o: src/ThreadNode.h src/ThreadNode.cpp Node.o RandomNodes.o
	$(CXX) $(GFLAGS) -c src/ThreadNode.cpp

RandomNodes.o: src/RandomNodes.h src/RandomNodes.cpp 
	$(CXX) $(GFLAGS) -c src/RandomNodes.cpp

Node.o: src/Node.h src/Node.cpp MessagePacket.o
	$(CXX) $(GFLAGS) -c src/Node.cpp

MessagePacket.o: src/MessagePacket.h src/MessagePacket.cpp TimeInterval.o
	$(CXX) $(GFLAGS) -c src/MessagePacket.cpp

TimeInterval.o: src/TimeInterval.h src/TimeInterval.cpp
	$(CXX) $(GFLAGS) -c src/TimeInterval.cpp

mailbox.o: src/mbox.h src/mailbox.cc MessagePacket.o
	$(CXX) $(GFLAGS) -c src/mailbox.cc

clean:
	rm -f driver
	rm *.o

run: driver
	./driver

valgrind: driver
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./driver
