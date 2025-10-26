#pragma once
//  server.h
#include <iostream>
#include <map>
#include <random>
#include <Windows.h>
#include "ringBuffer.h"

struct Server {
	Server();
	void createMailSlot();
	LPCSTR slotName;
	
private:
	HANDLE serverSlot;
	int NumToGuess;
	ringBuffer guessBuffer;
	map<int, HANDLE> playerMap;
};
 