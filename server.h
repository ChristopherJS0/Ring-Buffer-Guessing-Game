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
	void createMailSlot();
	void ProcessNewMessage(std::string msg);
	void mailslotListener();
	
	// Misc. Functions
	bool IsValidInt(std::string& str);
private:
	LPCSTR slotName;
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
 