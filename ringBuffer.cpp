#include "ringBuffer.h"

ringBuffer::ringBuffer() {
	ptrWrite = 0;
	ptrRead = 999;
	for (int i = 0; i < size; i++) {
		ring[i] = NULL;
	}
}

ringBuffer::~ringBuffer() {
	// Destructor logic if needed
	for (int i = 0; i < 10; i++) {
		ring[i] = NULL;
	}
}

void ringBuffer::Push(int value) {
	// If first, write to ptrWrite pos (0), then make ptrRead = 0
	if (initialized == false) {
		ring[ptrWrite] = value;
		ptrWrite = (ptrWrite + 1) % size;
		ptrRead = 0;
	}
	else {
		ring[ptrWrite] = value;
		while ((ptrWrite + 1) % size == ptrRead || ring[(ptrWrite + 1) % size] != NULL)
		{
			// Don't move to next, wait til that val either popped, otherwise deadlocked.
		}
		ptrWrite = (ptrWrite + 1) % size;
	}
	initialized = true;
}

int ringBuffer::Pop() {
	// Check if buffer is empty
	if (ring[ptrRead] == NULL) {
		return -1; // Indicate that the buffer is empty
	}
	else {
		int value = ring[ptrRead];
		ring[ptrRead] = NULL; // Clear the slot after reading
		while ((ptrRead + 1) % size == ptrWrite || ptrRead == 999) {
			// The next value in order isn't a valid one because ptrWrite
			// will write to it.
		}
		ptrRead = (ptrRead + 1) % size;
		return value;
	}	
}