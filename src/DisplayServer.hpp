#ifndef DISPLAY_SERVER_HEADER_GUARD
#define DISPLAY_SERVER_HEADER_GUARD

#include "NetworkHandler.hpp"

class DisplayServer
{
private:
    NetworkHandler networkHandler;
    int serverPort;

    void displayResult(const std::string& data);

public:
    DisplayServer(int port);
    ~DisplayServer();

    void start();
    void stop();
};

#endif