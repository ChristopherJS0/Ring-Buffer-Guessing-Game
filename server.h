#pragma once
//  server.h
#include <iostream>
#include <map>
#include <random>
#include <thread>
#include <set>
#include <Windows.h>
#include "ringBuffer.h"

struct Server {
	// Constructor and Destructor
	Server();
	~Server();

	// Game Processing Stuff
	void registerPlayer(std::string msg);
	void writeToPlayer(int playerID, std::string msg);

	// MailSlot Processing Stuff
	bool createMailSlot();
	void ProcessNewMessage(std::string msg);
	void ProcessGuess(std::string& msg, int playerID);
	void messageAllLosersAndWinner(int winnerID);

	// Getting the ID from msg
	int getIdFromMsg(std::string& msg);

	// Thread Functions
	void mailslotListener();
	void ringBufferPopper();


private:
	int NumToGuess;
	ringBuffer guessBuffer;
	map<int, HANDLE> playerMap;
	set<int> players;
	bool activeGame;

	// Threads
	HANDLE serverHandle;
	thread listenerThread;
	thread quitThread;
};
 