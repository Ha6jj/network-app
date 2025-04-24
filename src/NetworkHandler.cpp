#include "NetworkHandler.hpp"

void NetworkHandler::initializeWinsock()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(WSAGetLastError()));
    }
    isInitialized = true;
}

SOCKET& NetworkHandler::getActiveSocket()
{
    return isServer ? clientSocket : serverSocket;
}

NetworkHandler::NetworkHandler(bool asServer) : isServer(asServer)
{
    initializeWinsock();
}

NetworkHandler::~NetworkHandler()
{
    closeConnection();
    if (isInitialized)
    {
        WSACleanup();
    }
}

bool NetworkHandler::createSocket(int port, const std::string& ipAddress)
{
    if (isServer)
    {
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET)
        {
            throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
        }

        int opt = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)
        {
            closesocket(serverSocket);
            throw std::runtime_error("Setsockopt failed: " + std::to_string(WSAGetLastError()));
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = ipAddress.empty() ? INADDR_ANY : inet_addr(ipAddress.c_str());

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        {
            closesocket(serverSocket);
            throw std::runtime_error("Bind failed: " + std::to_string(WSAGetLastError()));
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
        {
            closesocket(serverSocket);
            throw std::runtime_error("Listen failed: " + std::to_string(WSAGetLastError()));
        }

        return true;
    }
    else
    {
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET)
        {
            throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
            
        if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0)
        {
            closesocket(serverSocket);
            throw std::runtime_error("Invalid address: " + ipAddress);
        }

        if (connect(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        {
            closesocket(serverSocket);
            throw std::runtime_error("Connect failed: " + std::to_string(WSAGetLastError()));
        }

        return true;
    }
}

bool NetworkHandler::acceptConnection()
{
    if (!isServer) return false;

    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("Accept failed: " + std::to_string(WSAGetLastError()));
    }
    return true;
}

std::string NetworkHandler::receiveData()
{
    char buffer[4096];
    SOCKET& activeSocket = getActiveSocket();
    
    if (activeSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("No active connection");
    }

    int bytesReceived = recv(activeSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        throw std::runtime_error("Receive failed: " + std::to_string(WSAGetLastError()));
    }
    if (bytesReceived == 0)
    {
        return "";
    }
    return std::string(buffer, bytesReceived);
}

bool NetworkHandler::sendData(const std::string& data)
{
    SOCKET& activeSocket = getActiveSocket();
        
    if (activeSocket == INVALID_SOCKET)
    {
        throw std::runtime_error("No active connection");
    }

    const char* buffer = data.c_str();
    int totalSent = 0;
    int bytesRemaining = data.size();
    int bytesSent = 0;

    while (totalSent < data.size())
    {
        bytesSent = send(activeSocket, buffer + totalSent, bytesRemaining, 0);
            
        if (bytesSent == SOCKET_ERROR)
        {
            throw std::runtime_error("Send failed: " + std::to_string(WSAGetLastError()));
        }
            
        totalSent += bytesSent;
        bytesRemaining -= bytesSent;
    }

    return totalSent == data.size();
}

void NetworkHandler::closeConnection()
{
    if (clientSocket != INVALID_SOCKET)
    {
        closesocket(clientSocket);
        clientSocket = INVALID_SOCKET;
    }
    if (serverSocket != INVALID_SOCKET)
    {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }
}