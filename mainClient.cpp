#include <iostream>
#include "client.h"
using namespace std;

int main() {
	// Create server instance
	cout << "Starting the client...\n";
	Player player1;
	player1.startListenerThread();
	player1.getInputFromUser();



	// Don't close the window immediately
	system("pause");
	return 0;
}