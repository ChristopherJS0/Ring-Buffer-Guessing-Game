#include "server.h"
#include "ringBuffer.h"
#include <iostream>

int main() {
	// Create server instance
	cout << "Starting the server...\n";
	Server server1;
	
	if (server1.createMailSlot()) {
		std::thread listenerThread = std::thread(&Server::mailslotListener, &server1);
		listenerThread.join();
	}
	else {
		cout << "Mailslot for server failed to create. Aborting server process.";
	}

	// Don't close the window immediately
	system("pause");
	return 0;
}