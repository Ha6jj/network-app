#include "DataProcessingServer.hpp"

bool DataProcessingServer::validateData(const std::string& data)
{
    if (data.empty()) return false;
        
    for (char c : data)
    {
        if (!(isalnum(c) || isspace(c) || c == '.' || c == ',' || c == '!' || c == '?'))
        {
            return false;
        }
    }
    return true;
}

std::string DataProcessingServer::processData(const std::string& data)
{
    std::stringstream ss(data);
    std::string word;
    std::unordered_set<std::string> uniqueWords;
    std::vector<std::string> words;

    while (ss >> word)
    {
        word.erase(std::remove_if(word.begin(), word.end(), [](char c) { return !isalpha(c); }), word.end());
            
        if (!word.empty())
        {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (uniqueWords.insert(word).second)
            {
                words.push_back(word);
            }
        }
    }

    std::string result;
    for (const auto& w : words)
    {
        if (!result.empty()) result += " ";
        result += w;
    }

    return result;
}


DataProcessingServer::DataProcessingServer(int inPort, int outPort, const std::string& outAddress)
    : inputPort(inPort), outputPort(outPort), outputAddress(outAddress), inputServer(true), outputClient(false) {}

DataProcessingServer::~DataProcessingServer()
{
    stop();
}

void DataProcessingServer::start()
{
    try
    {
        inputServer.createSocket(inputPort);
        std::cout << "DataProcessingServer started on port " << inputPort << std::endl;

        outputClient.createSocket(outputPort, outputAddress);
        std::cout << "Connected to DisplayServer at " << outputAddress << ":" << outputPort << std::endl;

        std::cout << "Waiting for client connection..." << std::endl;   
        inputServer.acceptConnection();
        std::cout << "Client connected" << std::endl;

        while (true)
        {
            try
            {
                std::string receivedData = inputServer.receiveData();
                std::cout << "Received data: " << receivedData << std::endl;

                if (!validateData(receivedData))
                {
                    inputServer.sendData("ERROR: Invalid data format");
                    std::cout << "Incorrect message received (ignored)" << std::endl;
                    continue;
                }

                inputServer.sendData("OK: Data received");

                std::string processedData = processData(receivedData);
                std::cout << "Processed data: " << processedData << std::endl;

                outputClient.sendData(processedData);
                std::cout << "Data sent to DisplayServer" << std::endl;

            }
            catch (const std::exception& e)
            {
                std::string errorInformation(e.what());
                std::cerr << "Processing error: " << errorInformation << std::endl;
                
                if (errorInformation.find("Receive") == 0)
                {
                    std::cout << "Waiting for client connection..." << std::endl;   
                    inputServer.acceptConnection();
                    std::cout << "Client connected" << std::endl;
                }
                else if (errorInformation.find("Send") == 0)
                {
                    std::cout << "Shutdown due to display side failure" << std::endl;
                    stop();
                    break;
                }
                else
                {
                    std::cout << "Unpredicted error" << std::endl;
                    stop();
                    break;
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Server error: " << e.what() << std::endl;
        stop();
    }
}

void DataProcessingServer::stop()
{
    inputServer.closeConnection();
    outputClient.closeConnection();
    std::cout << "DataProcessingServer stopped" << std::endl;
}