#include "DisplayServer.hpp"

void DisplayServer::displayResult(const std::string& data)
{
    std::cout << "=== New data received ===\n";
    std::cout << data << "\n";
    std::cout << "=========================\n\n";
}

DisplayServer::DisplayServer(int port)
    : serverPort(port), networkHandler(true) {}

DisplayServer::~DisplayServer()
{
    stop();
}

void DisplayServer::start()
{
    try
    {
        networkHandler.createSocket(serverPort);
        std::cout << "Display server ran on port: " << serverPort << std::endl;
        std::cout << "Waiting for data processing server to connect...\n";
        networkHandler.acceptConnection();
        std::cout << "Data processing server has connected\n";
    
        while (true)
        {
            try
            {
                std::string receivedData = networkHandler.receiveData();
                    
                if (!receivedData.empty())
                {
                    displayResult(receivedData);
                }
                else
                {
                    std::cout << "Data processing server closed connection\n";

                    std::cout << "Waiting for data processing server to connect...\n";
                    networkHandler.acceptConnection();
                    std::cout << "Data processing server has connected\n";
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "Data receiving error: " << e.what() << std::endl;

                std::cout << "Waiting for data processing server to connect...\n";
                networkHandler.acceptConnection();
                std::cout << "Data processing server has connected\n";
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
        stop();
    }
}

void DisplayServer::stop()
{
    networkHandler.closeConnection();
    std::cout << "Display server has stopped\n";
}