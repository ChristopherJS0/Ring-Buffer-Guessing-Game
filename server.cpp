#include "server.h"

// Server Constructor and Destructor
Server::Server() {
    NumToGuess = 8; // Just for testing purposes.
	activeGame = true;
	serverHandle = INVALID_HANDLE_VALUE;
}
Server::~Server() {
	// Close Threads and handles
    CloseHandle(serverHandle);
}

// Game Processsing Stuff
void Server::registerPlayer(std::string msg) {
    // Implementation for registering a player
    // This function would add the player to the playerMap
    // and perform any other necessary initialization.

    // Int Address of Player is the ID for now.

    std::string slotName = "\\\\.\\mailslot\\" + msg;

	LPCSTR lpcSlotName = slotName.c_str();
    HANDLE playerMailslot = CreateFileA(
        lpcSlotName,           // mailslot name
        GENERIC_WRITE,        // write access
        FILE_SHARE_READ,      // share mode
        NULL,                 // default security attributes
        OPEN_EXISTING,        // opens existing mailslot
        FILE_ATTRIBUTE_NORMAL,// normal attributes
		NULL);                // no template file
    
    if(playerMailslot == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create mailslot. Error: " << GetLastError() << "\n";
        return;
    }
    std::cout << "Player's mailslot created successfully!\n";
	// Getting the key to the map
    // find the underscore
	int i = 0;
    while (i < msg.size() && msg[i] != '_') {
        i++;
    }
	// everything after '_' is the address.
    std::string addrPart = "";
    if (i < msg.size() + 1) {
        addrPart = msg.substr(i + 1);  // skip the '_'
    }
    
	// Add to map using addressInt as key and mailslot handle as value
	playerMap[addrPart] = playerMailslot;
    
	players.insert(addrPart);
	cout << "Registered player with ID:" << addrPart << "\n";
    writeToPlayer(addrPart, "Welcome to the game!");
}
void Server::writeToPlayer(string playerID, std::string msg)
{
	// extra verification to ensure player exists
    if (playerMap.find(playerID) == playerMap.end()) {
        std::cout << "Player ID not found in map: " << playerID << "\n";
        std::cout << "wTP: Size of pMap: " << playerMap.size();
        return;
	}
    

	// Get the player's mailslot handle from the map
    DWORD bytesWritten;
    BOOL result = WriteFile(
        playerMap[playerID], // handle to mailslot
        msg.c_str(),         // message to write
        msg.size() + 1,      // size of message including null terminator
        &bytesWritten,       // number of bytes written
        NULL);               // not overlapped
    if (!result) {
        std::cout << "Failed to write to player mailslot. Error: " << GetLastError() << "\n";
    }
    else {
        std::cout << "Message sent to player: " << msg << "\n";
    }
}

// MailSlot Proccessing Stuff
bool Server::createMailSlot() {
    LPCSTR slotName = "\\\\.\\mailslot\\ServerSlot";

    // Create the mailslot
    serverHandle = CreateMailslotA(
        slotName,              // mailslot name
        0,                     // no max message size (0 = any)
        MAILSLOT_WAIT_FOREVER, // wait time
        NULL);                 // default security attributes

    if (serverHandle == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to create mailslot. Error: " << GetLastError() << "\n";
        return false;
    }

    std::cout << "Server mailslot created successfully!\n";
    return true;
}
void Server::ProcessNewMessage(std::string msg) {
	// Implementation for processing new messages
	// Depending on first char of msg, decide what type of msg it is.
    if (msg[0] == 'G') {
        // Guess message
        std::string guessStr = msg.substr(1);
		ProcessGuess(guessStr);
	}
    else if(msg[0] == 'R') {
        // Register message
        // Handled in registerPlayer
		std::string regInfo = msg.substr(1);
		registerPlayer(regInfo);
	}    
}
// Getting the ID from msg
void Server::ProcessGuess(std::string& msg) {
    std::string playerID = getIdFromMsg(msg); // Now msg only contains the guess part

    if (players.find(playerID) != players.end()) {
        cout << "Received guess from unregistered player ID: " << playerID << "\n";
        return;
	}
    cout << "Size of pMap: " << playerMap.size();


    try {
        int guess = std::stoi(msg);
        // Process the guess
        cout << "Player " << playerID << " guessed: " << guess << "\n";
        if (guess == NumToGuess) {
            writeToPlayer(playerID, "W"); // Correct
            cout << "Player " << playerID << " guessed correctly!\n";
        }
        else {
            writeToPlayer(playerID, "I"); // Incorrect
            cout << "Player " << playerID << " guessed incorrectly.\n";
		}
    }
    catch(...) {
        // Handle invalid guess
        cout << "Invalid guess received from player " << playerID << ": " << msg << "\n";
    }
}
// Message all players EXCEPT WINNER that they lost.
void Server::messageAllLosers(std::string winnerID) {
    for (const auto& pair : playerMap) {
        if (pair.first != winnerID) {
            writeToPlayer(pair.first, "L"); // Lose message
        }
        else {
			writeToPlayer(pair.first, "W"); // Win message
        }
    }
}

// Getting the guess from msg
std::string Server::getIdFromMsg(std::string& msg) {
    // Assuming msg format is "G<ID>_<Guess>"
    char iter;
	string id = "";
    int ind = 0;
    while (ind < msg.size() && msg[ind] != '_') {
        iter = msg[ind];
		id += iter;
        ind++;
	}
	msg = msg.substr(ind + 1); // Update msg to only contain the guess part
    return id;
}

// THREAD: to listen for msgs in server mailslot.
void Server::mailslotListener() {
    char buffer[512];
    DWORD bytesRead;

    while (activeGame) {
        // Check if there are messages waiting
        DWORD nextSize, messageCount;
        BOOL success = GetMailslotInfo(serverHandle, NULL, &nextSize, &messageCount, NULL);

        if (!success) {
            // std::cerr << "GetMailslotInfo failed: " << GetLastError() << std::endl;
            break;
        }

        // If there are messages, read them
        while (messageCount > 0) {
            if (nextSize != MAILSLOT_NO_MESSAGE) {
                DWORD bytesReadActual;
                BOOL readOk = ReadFile(serverHandle, buffer, sizeof(buffer) - 1, &bytesReadActual, NULL);
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
            success = GetMailslotInfo(serverHandle, NULL, &nextSize, &messageCount, NULL);
            if (!success) { break; }
        }

        Sleep(100); // prevent CPU overuse
    }
    // std::cout << "Listener thread ending.\n";
}


