#include "./src/DisplayServer.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <port>\n";
        return 1;
    }

    try
    {
        int port = std::stoi(argv[1]);
        DisplayServer server(port);
        server.start();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}