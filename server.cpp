#include "server.h"

Server::Server() {
    NumToGuess = 8; // Just for testing purposes.
	createMailSlot();
}

void Server::createMailSlot() {
    slotName = "\\\\.\\mailslot\\ServerSlot";

    // Create the mailslot
    serverSlot = CreateMailslotA(
        slotName,              // mailslot name
        0,                     // no max message size (0 = any)
        MAILSLOT_WAIT_FOREVER, // wait time
        NULL);                 // default security attributes

    if (serverSlot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create mailslot. Error: " << GetLastError() << "\n";
        return;
    }

    std::cout << "Server mailslot created successfully!\n";
    // Keep program open for now
}
