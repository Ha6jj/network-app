## An application for network interaction between the client and data processing/display servers

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.12+-green.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)

## 📦 Project structure
project/
├── build/
│└── bin/ # Executable files
│   ├── client.exe
│   ├── display.exe
│   ├── network_test.exe # Network interaction testing
│   ├── processing_test.exe # Data processing testing
│   └── server.exe
├── lib/ # GTest static library
├── src/ # Source code
│ ├── ClientApp.cpp
│ ├── DataProcessingServer.cpp
│ ├── DisplayServer.cpp
│ └── NetworkHandler.cpp # Network Core
├── tests/ # Unit tests
│ └── network_test.cpp
│ └── processing_test.cpp
├── CMakeLists.txt # Config
├── client.cpp
├── display.cpp
├── server.cpp
└── README.md


## 🛠 Установка и сборка

### Требования
- CMake 3.12+
- C++17 (GCC, Clang)
- (Optional) Google Test for tests

## ⚙️ Сборка

git clone https://github.com/Ha6jj/network-app.git
cd network-app

mkdir build
cd build

cmake ..

make

##🚀 Launch

### Command Line Options

| Component       | Argument               |
|-----------------|------------------------|
| **client**      | `<server adress> <port>`     |
| **server**      | `<input_port> <output_address> <output_port>`        |
| **display**     | `<port>`        |
