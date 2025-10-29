#include <iostream>
#include "client.h"
using namespace std;

int main() {
	// Create server instance
	cout << "Starting the client...\n";
	Player player1;
	if (player1.registerToServer()) {
		player1.startListenerThread();
		player1.startInputThread();
	}
	else {
		cout << "Failed to register to server. Aborting client process.";
	}

	// Don't close the window immediately
	system("pause");
	return 0;
}