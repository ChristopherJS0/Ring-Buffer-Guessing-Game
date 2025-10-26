#include "ringBuffer.h"

ringBuffer::ringBuffer() {
	ptrWrite = 0;
}

ringBuffer::~ringBuffer() {
	// Destructor logic if needed
	for (int i = 0; i < 10; i++) {
		ring[i] = NULL;
	}
}

void ringBuffer::Push(int value) {
	
	ring[ptrWrite] = value;
	ptrWrite = (ptrWrite + 1) % size;

}

int ringBuffer::Pop() {
	// Check if buffer is empty
	if (ring[ptrRead] == NULL) {
		return -1; // Indicate that the buffer is empty
	}
	// Locking method to prevent read and write at the same time
	else if (ptrRead == ptrWrite) {
		return -1; // Indicate that the buffer is empty
	}
	else {
		int value = ring[ptrRead];
		ring[ptrRead] = NULL; // Clear the slot after reading
		ptrRead = (ptrRead + 1) % size;
		return value;
	}
	
}