TestAllocator: TestAllocator.cpp StackAllocator.hpp
	g++-6 -std=c++14 -fsanitize=address -g -o TestAllocator TestAllocator.cpp
