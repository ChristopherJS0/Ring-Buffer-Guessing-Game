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
	bool registerToServer();
	bool SendMessageToS(std::string& msg);

		// Receive Stuff
	void ProcessNewMessage(std::string msg);

	// Thread functions
	void listenToServer();
	void getInputFromUser();


private:
	HANDLE clientSlot;
	HANDLE serverSlot;
	std::string playerName;
	std::string playerID;

	bool active;
};
