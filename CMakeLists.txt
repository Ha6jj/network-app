cmake_minimum_required(VERSION 3.12)
project(NetworkApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    set(WS2_LIB ws2_32)
endif()

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


target_link_libraries(client ${WS2_LIB})
target_link_libraries(server ${WS2_LIB})
target_link_libraries(display ${WS2_LIB})

set(GTEST_INCLUDE_DIRS "C:\\Program Files\\googletest\\googletest\\include")
set(GTEST_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/libgtest.a")

target_include_directories(network_test PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(network_test ${GTEST_LIBRARIES} ${WS2_LIB})
target_include_directories(processing_test PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(processing_test ${GTEST_LIBRARIES} ${WS2_LIB})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)