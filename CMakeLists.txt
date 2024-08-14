cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


project(testWeb)

find_package(libpqxx CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

add_executable(testWB main.cpp)
add_executable(crw crow.cpp)

target_link_libraries(testWB PRIVATE libpqxx::pqxx)
target_link_libraries(crw Crow::Crow asio::asio)