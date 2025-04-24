#include "./src/ClientApp.hpp"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <server adress> <port>" << std::endl;
        return 1;
    }

    try
    {
        std::string serverAddress = argv[1];
        int serverPort = std::stoi(argv[2]);

        ClientApp app(serverAddress, serverPort);
        app.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}