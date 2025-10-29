#include <iostream>
#include "client.h"
using namespace std;

int main() {
	// Create server instance
	cout << "Starting the client...\n";
	Player player1;
	if (player1.registerToServer()) {
		std::thread listenerThread = std::thread(&Player::listenToServer, &player1);
		std::thread inputThread = std::thread(&Player::getInputFromUser, &player1);
		listenerThread.join();
		inputThread.join();
	}
	else {
		cout << "Failed to register to server. Aborting client process.";
	}

	// Don't close the window immediately
	system("pause");
	return 0;
}