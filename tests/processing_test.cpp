#include "gtest/gtest.h"
#include "../src/DataProcessingServer.hpp"
#include "../src/NetworkHandler.hpp"
#include <algorithm>

using namespace std::chrono_literals;

class DataProcessingTest : public ::testing::Test
{
protected:
    DataProcessingServer server;
    DataProcessingTest() : server(7777, 8888, std::string("1.2.3.4")) {}
};

TEST_F(DataProcessingTest, ValidatesNormalText)
{
    EXPECT_TRUE(server.validateData("No Frameworks?"));
}

TEST_F(DataProcessingTest, InvalidDataTest)
{
    EXPECT_FALSE(server.validateData("\x01\x02"));
    EXPECT_FALSE(server.validateData(""));
}

TEST_F(DataProcessingTest, RemovesDuplicates)
{
    EXPECT_EQ(server.processData("a a b b c c"), "a b c");
    EXPECT_EQ(server.processData("word word word"), "word");
    EXPECT_EQ(server.processData("a b c b a"), "a b c");
}

TEST_F(DataProcessingTest, RemovesCaseDuplicates)
{
    EXPECT_EQ(server.processData("python PyThon PYTHON"), "python");
}

TEST_F(DataProcessingTest, RemovesPunctuation)
{
    EXPECT_EQ(server.processData("word, word. word!"), "word");
    EXPECT_EQ(server.processData("a-b a-b a-b"), "ab");
}

TEST_F(DataProcessingTest, LargeInputTest)
{
    std::string input;
    for (int i = 0; i < 10000; ++i)
    {
        input += "a ";
    }
    
    std::string result = server.processData(input);
    EXPECT_EQ(result, "a");
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}