shm: main.o shared_memory.o
	g++ -o shm main.o shared_memory.o
main.o: main.cc
	g++ -c main.cc
shared_memory.o: shared_memory.cc
	g++ -c shared_memory.cc