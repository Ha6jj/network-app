#ifndef NETWORK_HANDLER_HEADER_GUARD
#define NETWORK_HANDLER_HEADER_GUARD

#include <string>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    using socket_type = SOCKET;
    constexpr socket_type INVALID_SOCKET_VALUE = INVALID_SOCKET;
    constexpr int SOCKET_ERROR_VALUE = SOCKET_ERROR;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <cerrno>
    using socket_type = int;
    constexpr socket_type INVALID_SOCKET_VALUE = -1;
    constexpr int SOCKET_ERROR_VALUE = -1;
#endif

class NetworkHandler
{
private:
#ifdef _WIN32
    WSADATA wsaData;
#endif
    socket_type serverSocket = INVALID_SOCKET_VALUE;
    socket_type clientSocket = INVALID_SOCKET_VALUE;
    bool isServer = false;
    bool isInitialized = false;

    void initializeNetwork();
    socket_type& getActiveSocket();

public:
    NetworkHandler(bool asServer = false);
    ~NetworkHandler();

    bool createSocket(int port, const std::string& ipAddress = "");
    bool acceptConnection();

    std::string receiveData();
    bool sendData(const std::string& data);

    void closeConnection();
};

#endif // NETWORK_HANDLER_HEADER_GUARD