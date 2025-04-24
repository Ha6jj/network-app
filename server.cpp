#include "./src/DataProcessingServer.hpp"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <input_port> <output_address> <output_port>" << std::endl;
        return 1;
    }

    try
    {
        int inputPort = std::stoi(argv[1]);
        std::string outputAddress = argv[2];
        int outputPort = std::stoi(argv[3]);

        DataProcessingServer server(inputPort, outputPort, outputAddress);
        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}