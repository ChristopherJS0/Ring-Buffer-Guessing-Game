#pragma once
//	client.h
#include <iostream>
#include <windows.h>
#include <string>

// Also will be known as client.
class Player {
public:
	Player();
	void NameAndMailSlot();
	void registerToServer();

	void SendMessage(std::string msg);
	
private:
	HANDLE clientSlot;
	std::string playerName;
};
