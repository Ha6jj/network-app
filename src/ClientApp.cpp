#include "ClientApp.hpp"

bool ClientApp::connectToServer()
{
    try
    {
        networkHandler.createSocket(serverPort, serverAddress);
        isConnected = true;
        std::cout << "Connected to server " << serverAddress << ":" << serverPort << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Connection error: " << e.what() << std::endl;
        isConnected = false;
        return false;
    }
}

std::string ClientApp::getUserInput()
{
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);
    return input;
}

bool ClientApp::sendWithConfirmation(const std::string& data)
{
    if (!isConnected) return false;
    
    try
    {
        networkHandler.sendData(data);
        std::cout << "Data sent, waiting confirmation..." << std::endl;

        std::string confirmation = networkHandler.receiveData();
        std::cout << "Server confirmation: " << confirmation << std::endl;
        
        if (confirmation.find("OK:") == 0)
        {
            return true;
        }
        else if (confirmation.find("ERROR:") == 0)
        {
            std::cerr << "Error from server: " << confirmation.substr(6) << std::endl;
            return false;
        }

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Sending/receiving error: " << e.what() << std::endl;
        isConnected = false;
        return false;
    }
}

ClientApp::ClientApp(const std::string& address, int port) 
    : serverAddress(address), serverPort(port) {}

void ClientApp::run()
{
    if (!connectToServer())
    {
        std::cerr << "Connection to server failed. Stopping application ..." << std::endl;
        return;
    }

    while (isConnected)
    {
        std::string input = getUserInput();
        
        if (input.empty()) continue;
        
        if (input == "!exit")
        {
            std::cout << "Client shutdown..." << std::endl;
            break;
        }

        if (!sendWithConfirmation(input))
        {
            std::cerr << "Sending data failed. Please, try again" << std::endl;
        }
    }

    networkHandler.closeConnection();
}