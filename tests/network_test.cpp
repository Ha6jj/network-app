#include "gtest/gtest.h"
#include "../src/NetworkHandler.hpp"
#include <thread>
#include <atomic>

using namespace std::chrono_literals;

class NetworkHandlerTest : public ::testing::Test
{
protected:
    int test_port = 7777;
    const std::string test_address = "127.0.0.1";
};

TEST_F(NetworkHandlerTest, BasicSocketOperations)
{
    NetworkHandler handler(true);
    EXPECT_NO_THROW(handler.createSocket(test_port));
    EXPECT_NO_THROW(handler.closeConnection());
}

TEST_F(NetworkHandlerTest, DoubleClose)
{
    NetworkHandler handler(true);
    handler.createSocket(test_port);
    EXPECT_NO_THROW(handler.closeConnection());
    EXPECT_NO_THROW(handler.closeConnection());
}

TEST_F(NetworkHandlerTest, HandleMultipleClients)
{
    NetworkHandler server(true);
    server.createSocket(test_port);

    std::atomic<int> connections{0};
    auto new_client = [&]()
    {
        NetworkHandler client(false);
        if (client.createSocket(test_port, test_address))
        {
            connections++;
        }
    };

    std::thread t1(new_client);
    std::thread t2(new_client);
    std::this_thread::sleep_for(100ms);
    EXPECT_TRUE(server.acceptConnection());
    EXPECT_TRUE(server.acceptConnection());
    
    t1.join();
    t2.join();
    EXPECT_EQ(connections, 2);
}

TEST_F(NetworkHandlerTest, HandlingLargeData)
{
    NetworkHandler server(true), client(false);
    server.createSocket(test_port);
    client.createSocket(test_port, test_address);
    server.acceptConnection();

    const std::string large_data(4000, 'A');
    EXPECT_TRUE(client.sendData(large_data));
    
    std::string received = server.receiveData();
    EXPECT_EQ(received.size(), large_data.size());
    EXPECT_EQ(received.front(), 'A');
    EXPECT_EQ(received.back(), 'A');
}

TEST_F(NetworkHandlerTest, HandlingPartialData)
{
    NetworkHandler server(true), client(false);
    server.createSocket(test_port);
    client.createSocket(test_port, test_address);
    server.acceptConnection();

    client.sendData("Bada");
    std::this_thread::sleep_for(50ms);
    client.sendData("Boom");
    
    EXPECT_EQ(server.receiveData(), "BadaBoom");
}

TEST_F(NetworkHandlerTest, ReceiveFromDisconnectedClient)
{
    NetworkHandler server(true), client(false);
    server.createSocket(test_port);
    client.createSocket(test_port, test_address);
    server.acceptConnection();
    client.closeConnection();
    
    EXPECT_TRUE(server.receiveData().empty());
}

TEST_F(NetworkHandlerTest, ServerHeavyLoad)
{
    NetworkHandler server(true);
    server.createSocket(test_port);
    
    int clients_number = 50;
    std::vector<std::thread> clients;
    std::atomic<int> clients_connected{0};
    
    std::string message = "msg";
    auto client_task = [&]()
    {
        NetworkHandler client(false);
        if (client.createSocket(test_port, test_address))
        {
            if (client.sendData(message))
            {
                clients_connected++;
            }
        }
    };
    
    for (int i = 0; i < clients_number; i++)
    {
        clients.emplace_back(client_task);
    }
    
    for (int i = 0; i < clients_number; i++)
    {
        if (server.acceptConnection())
        {
            server.receiveData();
        }
    }
    
    for (auto& t : clients)
    {
        t.join();
    }
    EXPECT_EQ(clients_connected, clients_number);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}