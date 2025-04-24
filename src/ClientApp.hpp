#ifndef CLIENT_APP_HEADER_GUARD
#define CLIENT_APP_HEADER_GUARD

#include "NetworkHandler.hpp"

class ClientApp
{
private:
    NetworkHandler networkHandler;
    std::string serverAddress;
    int serverPort;
    bool isConnected = false;

    bool connectToServer();

    std::string getUserInput();

    bool sendWithConfirmation(const std::string& data);

public:
    ClientApp(const std::string& address, int port);

    void run();
};

#endif