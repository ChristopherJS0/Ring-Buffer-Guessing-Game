#pragma once
//  server.h
#include <iostream>
#include <map>
#include <random>
#include <thread>
#include <Windows.h>
#include "ringBuffer.h"

struct Server {
	Server();
	~Server();

	void registerPlayer(std::string msg);
	void writeToPlayer(string playerID, std::string msg);

	void createMailSlot();
	void ProcessNewMessage(std::string msg);
	void mailslotListener();
	LPCSTR slotName;
	
private:
	HANDLE serverSlot;
	int NumToGuess;
	ringBuffer guessBuffer;
	map<string, HANDLE> playerMap;
	bool activeGame;


	// Threads
	HANDLE serverHandle;
	thread listenerThread;
	thread quitThread;
};
 