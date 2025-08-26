#include "NetworkHandler.hpp"
#include <cstring>

void NetworkHandler::initializeNetwork()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(WSAGetLastError()));
    }
#endif
    isInitialized = true;
}

socket_type& NetworkHandler::getActiveSocket()
{
    return isServer ? clientSocket : serverSocket;
}

NetworkHandler::NetworkHandler(bool asServer) : isServer(asServer)
{
    initializeNetwork();
}

NetworkHandler::~NetworkHandler()
{
    closeConnection();
#ifdef _WIN32
    if (isInitialized)
    {
        WSACleanup();
    }
#endif
}

bool NetworkHandler::createSocket(int port, const std::string& ipAddress)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET_VALUE)
    {
#ifdef _WIN32
        throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
#else
        throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
#endif
    }

    if (isServer)
    {
        int opt = 1;
#ifdef _WIN32
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) == SOCKET_ERROR_VALUE)
#else
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
#endif
        {
            closeConnection();
#ifdef _WIN32
            throw std::runtime_error("Setsockopt failed: " + std::to_string(WSAGetLastError()));
#else
            throw std::runtime_error("Setsockopt failed: " + std::string(strerror(errno)));
#endif
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = ipAddress.empty() ? INADDR_ANY : inet_addr(ipAddress.c_str());

        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR_VALUE)
        {
            closeConnection();
#ifdef _WIN32
            throw std::runtime_error("Bind failed: " + std::to_string(WSAGetLastError()));
#else
            throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
#endif
        }

        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR_VALUE)
        {
            closeConnection();
#ifdef _WIN32
            throw std::runtime_error("Listen failed: " + std::to_string(WSAGetLastError()));
#else
            throw std::runtime_error("Listen failed: " + std::string(strerror(errno)));
#endif
        }

        return true;
    }
    else
    {
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
            
        if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0)
        {
            closeConnection();
            throw std::runtime_error("Invalid address: " + ipAddress);
        }

        if (connect(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR_VALUE)
        {
            closeConnection();
#ifdef _WIN32
            throw std::runtime_error("Connect failed: " + std::to_string(WSAGetLastError()));
#else
            throw std::runtime_error("Connect failed: " + std::string(strerror(errno)));
#endif
        }

        return true;
    }
}

bool NetworkHandler::acceptConnection()
{
    if (!isServer) return false;

    clientSocket = accept(serverSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET_VALUE)
    {
#ifdef _WIN32
        throw std::runtime_error("Accept failed: " + std::to_string(WSAGetLastError()));
#else
        throw std::runtime_error("Accept failed: " + std::string(strerror(errno)));
#endif
    }
    return true;
}

std::string NetworkHandler::receiveData()
{
    char buffer[4096];
    socket_type& activeSocket = getActiveSocket();
    
    if (activeSocket == INVALID_SOCKET_VALUE)
    {
        throw std::runtime_error("No active connection");
    }

    int bytesReceived = recv(activeSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR_VALUE)
    {
#ifdef _WIN32
        throw std::runtime_error("Receive failed: " + std::to_string(WSAGetLastError()));
#else
        throw std::runtime_error("Receive failed: " + std::string(strerror(errno)));
#endif
    }
    if (bytesReceived == 0)
    {
        return "";
    }
    return std::string(buffer, bytesReceived);
}

bool NetworkHandler::sendData(const std::string& data)
{
    socket_type& activeSocket = getActiveSocket();
        
    if (activeSocket == INVALID_SOCKET_VALUE)
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
            
        if (bytesSent == SOCKET_ERROR_VALUE)
        {
#ifdef _WIN32
            throw std::runtime_error("Send failed: " + std::to_string(WSAGetLastError()));
#else
            throw std::runtime_error("Send failed: " + std::string(strerror(errno)));
#endif
        }
            
        totalSent += bytesSent;
        bytesRemaining -= bytesSent;
    }

    return totalSent == data.size();
}

void NetworkHandler::closeConnection()
{
    if (clientSocket != INVALID_SOCKET_VALUE)
    {
#ifdef _WIN32
        closesocket(clientSocket);
#else
        close(clientSocket);
#endif
        clientSocket = INVALID_SOCKET_VALUE;
    }
    if (serverSocket != INVALID_SOCKET_VALUE)
    {
#ifdef _WIN32
        closesocket(serverSocket);
#else
        close(serverSocket);
#endif
        serverSocket = INVALID_SOCKET_VALUE;
    }
}