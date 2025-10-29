#pragma once
//  server.h
#include <iostream>
#include <map>
#include <random>
#include <thread>
#include <Windows.h>
#include "ringBuffer.h"

struct Server {
	// Constructor and Destructor
	Server();
	~Server();

	// Game Processing Stuff
	void registerPlayer(std::string msg);
	void writeToPlayer(string playerID, std::string msg);

	// MailSlot Processing Stuff
	bool createMailSlot();
	void ProcessNewMessage(std::string msg);
	void mailslotListener();

	// Thread Starters
	void startListenerThread();


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
 