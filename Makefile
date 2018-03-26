CC=g++-6
CFLAGS=-std=c++14 -g -O3
TestAllocator: TestAllocator.cpp StackAllocator.hpp
	$(CC) -o TestAllocator TestAllocator.cpp $(CFLAGS)
