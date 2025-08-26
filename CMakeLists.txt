cmake_minimum_required(VERSION 3.12)
project(NetworkApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_SOURCES src/NetworkHandler.cpp)
set(CLIENT_SOURCES src/ClientApp.cpp client.cpp)
set(SERVER_SOURCES src/DataProcessingServer.cpp server.cpp)
set(DISPLAY_SOURCES src/DisplayServer.cpp display.cpp)
set(TEST_NETWORK_SOURCES tests/network_test.cpp)
set(TEST_PROCESSING_SOURCES tests/processing_test.cpp)


add_executable(client ${CLIENT_SOURCES} ${COMMON_SOURCES})
add_executable(server ${SERVER_SOURCES} ${COMMON_SOURCES})
add_executable(display ${DISPLAY_SOURCES} ${COMMON_SOURCES})

add_executable(network_test ${TEST_NETWORK_SOURCES} ${COMMON_SOURCES})
add_executable(processing_test ${TEST_PROCESSING_SOURCES} ${COMMON_SOURCES} src/DataProcessingServer.cpp)

if (WIN32)
    target_link_libraries(client ws2_32)
    target_link_libraries(server ws2_32)
    target_link_libraries(display ws2_32)
    target_link_libraries(network_test ws2_32)
    target_link_libraries(processing_test ws2_32)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(GTest REQUIRED)
if(NOT TARGET GTest::gtest)
    message(FATAL_ERROR "GTest not found!")
endif()

target_link_libraries(network_test PRIVATE GTest::gtest GTest::gtest_main pthread)
target_link_libraries(processing_test PRIVATE GTest::gtest GTest::gtest_main pthread)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)