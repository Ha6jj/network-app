#ifndef DATA_PROCESSING_SERVER_HEADER_GUARD
#define DATA_PROCESSING_SERVER_HEADER_GUARD

#include <sstream>
#include <unordered_set>
#include <algorithm>
#include "NetworkHandler.hpp"

class DataProcessingServer
{
private:
    NetworkHandler inputServer;
    NetworkHandler outputClient;
    int inputPort;
    int outputPort;
    std::string outputAddress;

public:
    DataProcessingServer(int inPort, int outPort, const std::string& outAddress);
    ~DataProcessingServer();

    bool validateData(const std::string& data);
    std::string processData(const std::string& data);

    void start();
    void stop();
};

#endif