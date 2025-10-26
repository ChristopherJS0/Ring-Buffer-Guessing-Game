 #pragma once
#include <string>
using namespace std;

struct ringBuffer {
	ringBuffer();
	~ringBuffer();
	void Push(int value);
	int Pop();

	bool initialized = false; // To check if the buffer has been initialized
	int ptrWrite;
	int ptrRead;
	int ring[10];
	const int size = 10;
};