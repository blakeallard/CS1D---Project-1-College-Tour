cmake_minimum_required(VERSION 3.10)
project("CS1D_GROUP_PROJECT")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES src/main.cpp
        src/RouteRegister.h
        src/Controllers.h
        src/Controllers/UniversityController.cpp
        src/Controllers/UserController.cpp
        src/Controllers/AdminController.cpp
        src/Controllers/TourController.cpp
        src/database.cpp
        src/Wallet.cpp
)

add_executable(execBinary ${PROJECT_SOURCES})

target_include_directories(execBinary PUBLIC ${CMAKE_SOURCE_DIR}/include/ASIO_headers
PUBLIC ${CMAKE_SOURCE_DIR}/include/crow_headers
PUBLIC ${CMAKE_SOURCE_DIR}/include
)

find_package(SQLite3 REQUIRED)
target_link_libraries(execBinary SQLite::SQLite3)
