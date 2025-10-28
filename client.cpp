#include "client.h"
// Player Constructor and Destructor
Player::Player() {
    NameAndMailSlot();
	// Send playere to server or perform other initialization as needed
	registerToServer();
}
Player::~Player() {
    CloseHandle(clientSlot);
	CloseHandle(serverSlot);
    if (listenerThread.joinable()) {
        listenerThread.join();
	}
    if (inputThread.joinable()) {
        inputThread.join();
	}
}

// Game Processsing Stuff
void Player::NameAndMailSlot() {
	std::cout << "Please enter your name and hit enter once done: " << std::endl;
	std::getline(std::cin, playerName);
	if (playerName == "") {
        NameAndMailSlot();
	}

	//Mailslot = Playername + memoryAddress
    playerID = std::to_string(reinterpret_cast<uintptr_t>(&playerName));

    std::string slotName = "\\\\.\\mailslot\\"
        + playerName + "_" + playerID;
    LPCSTR lpcSlotName = slotName.c_str();

    // Create the mailslot
    clientSlot = CreateMailslotA(
        lpcSlotName,           // mailslot name
        0,                     // no max message size (0 = any)
        MAILSLOT_WAIT_FOREVER, // wait time
        NULL);                 // default security attributes

    if (clientSlot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create mailslot. Error: " << GetLastError() << "\n";
        return;
    }
    std::cout << playerName << "'s mailslot created successfully!\n";
}
void Player::registerToServer() {
	// Implementation for registering to the server
	std::string registrationMsg = "R" + playerName + "_" +
        std::to_string(reinterpret_cast<uintptr_t>(&playerName));

    LPCSTR address = "\\\\.\\mailslot\\ServerSlot";
    serverSlot = CreateFileA(
        address,               // mailslot name
        GENERIC_WRITE,        // write access
        FILE_SHARE_READ,      // share mode
        NULL,                 // default security attributes
        OPEN_EXISTING,        // opens existing mailslot
        FILE_ATTRIBUTE_NORMAL,// normal attributes
        NULL);                // no template file

    if (serverSlot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open server mailslot. Error: " << GetLastError() << "\n";
        return;
    }
    SendMessageToS(registrationMsg);
}
void Player::SendMessageToS(std::string msg) {

    DWORD bytesWritten;
    BOOL result = WriteFile(
        serverSlot,           // handle to mailslot
        msg.c_str(),         // message to write
        msg.size() + 1,      // size of message including null terminator
        &bytesWritten,       // number of bytes written
        NULL);               // not overlapped
    if (!result) {
        std::cout << "Failed to write to server mailslot. Error: " << GetLastError() << "\n";
    } else {
        std::cout << "Message sent to server: " << msg << "\n";
    }
}

// Receive Stuff
void Player::ProcessNewMessage(std::string msg) {
    // Implementation for processing new messages from server
    std::cout << "Message from server: " << msg << std::endl;
}

// THREAD functions
void Player::startListenerThread() {
	listenerThread = std::thread(&Player::listenToServer, this);
}
void Player::startInputThread() {
    inputThread = std::thread(&Player::getInputFromUser, this);
}
void Player::listenToServer() {
    // Implementation for listening to server messages
    char buffer[512];
    DWORD bytesRead;

    while (true) {
        // Check if there are messages waiting
        DWORD nextSize, messageCount;
        BOOL success = GetMailslotInfo(clientSlot, NULL, &nextSize, &messageCount, NULL);

        if (!success) {
            // std::cerr << "GetMailslotInfo failed: " << GetLastError() << std::endl;
            break;
        }

        // If there are messages, read them
        while (messageCount > 0) {
            if (nextSize != MAILSLOT_NO_MESSAGE) {
                DWORD bytesReadActual;
                BOOL readOk = ReadFile(clientSlot, buffer, sizeof(buffer) - 1, &bytesReadActual, NULL);
                if (readOk) {
                    buffer[bytesReadActual] = '\0'; // null-terminate
                    // std::cout << "Received: " << buffer << std::endl;

                    // Process the message
                    std::string msg(buffer, bytesReadActual);
                    ProcessNewMessage(msg);
                }
                else {
                    // std::cerr << "ReadFile failed: " << GetLastError() << std::endl;
                }
            }

            // Check again for more messages
            success = GetMailslotInfo(clientSlot, NULL, &nextSize, &messageCount, NULL);
            if (!success) { break; }
        }

        Sleep(100); // prevent CPU overuse
    }


}

// TO-DO: Implement user input handling
void Player::getInputFromUser() {
    // Implementation for getting user input
    while (true) {
        std::string userInput;
        std::cout << "Enter your guess: ";
        std::getline(std::cin, userInput);
        if (!userInput.empty()) {
            std::string guessMsg = "G" + userInput;
            SendMessageToS(guessMsg);
        }
	}
}