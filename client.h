#pragma once
//	client.h
#include <iostream>
#include <windows.h>
#include <thread>
#include <string>

// Also will be known as client.
class Player {
public:
	// Constructor and Destructor	
	Player();
	~Player();

	//Mailslot Stuff
		// Send Stuff
	void NameAndMailSlot();
	void registerToServer();
	void SendMessageToS(std::string msg);

		// Receive Stuff
	void ProcessNewMessage(std::string msg);

	// Thread functions
	void startListenerThread();
	void startInputThread();

	void listenToServer();
	void getInputFromUser();

	//Misc. Functions

private:
	HANDLE clientSlot;
	HANDLE serverSlot;
	std::string playerName;
	std::string playerID;

	// Threads
	std::thread listenerThread;
	std::thread inputThread;

};
