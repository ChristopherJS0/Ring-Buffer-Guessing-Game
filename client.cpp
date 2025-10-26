#include "client.h"

Player::Player() {
    NameAndMailSlot();
	// Send playere to server or perform other initialization as needed

}

void Player::NameAndMailSlot() {
	std::cout << "Please enter your name and hit enter once done: " << std::endl;
	std::getline(std::cin, playerName);
	if (playerName == "") {
        NameAndMailSlot();
	}

	//Mailslot = Playername + memoryAddress
    std::string slotName = "\\\\.\\mailslot\\" 
        + playerName + "_" + std::to_string(reinterpret_cast<uintptr_t>(&playerName));
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
	std::string registrationMsg = "REGISTER->" + 
		playerName + "\n" + 
        std::to_string(reinterpret_cast<uintptr_t>(&playerName));
	SendMessage(registrationMsg);

}
void Player::SendMessage(std::string msg) {
	// Implementation for preparing a message to send to the server
	// Might be a little weird opening and closing the mailslot each time,
	// but it works for now.
    LPCSTR address = "\\\\.\\mailslot\\ServerSlot";
    HANDLE serverSlot = CreateFileA(
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
	CloseHandle(serverSlot);
}