cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


project(testWeb)

find_package(libpqxx CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(libmodbus CONFIG REQUIRED)


add_executable( testWB 
    src/main.cpp
    src/db.cpp
)

target_link_libraries(testWB PRIVATE 
    libpqxx::pqxx 
    Crow::Crow
    asio::asio
    modbus
)
