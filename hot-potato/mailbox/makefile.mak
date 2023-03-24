CXX = g++ -pthread
GFLAGS = -g -std=c++11
OBJECTS = mailbox.o
FILES = mailbox.cc

mailbox.o: mailbox.cc
	$(CXX) $(GFLAGS) -c mailbox.cc

clean:
	rm -f mailbox.o
	rm *.o

valgrind: driver
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./driver