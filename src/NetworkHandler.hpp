#ifndef NETWORK_HANDLER_HEADER_GUARD
#define NETWORK_HANDLER_HEADER_GUARD

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>

class NetworkHandler
{
private:
    WSADATA wsaData;
    SOCKET serverSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    bool isServer = false;
    bool isInitialized = false;

    void initializeWinsock();
    SOCKET& getActiveSocket();

public:
    NetworkHandler(bool asServer = false);
    ~NetworkHandler();

    bool createSocket(int port, const std::string& ipAddress = "");
    bool acceptConnection();

    std::string receiveData();
    bool sendData(const std::string& data);

    void closeConnection();
};

#endif