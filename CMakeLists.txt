cmake_minimum_required(VERSION 3.10)
project("CS1D_GROUP_PROJECT")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor/sqlite3)
add_subdirectory(src/Database)
add_subdirectory(src/Controllers)
add_subdirectory(include/crow)

set(PROJECT_SOURCES src/main.cpp
        src/RouteRegister.h
        src/database.cpp
        src/Wallet.cpp
        src/StudentPurchases.cpp

)

add_executable(execBinary ${PROJECT_SOURCES})

target_include_directories(execBinary
    PUBLIC
        sqlite3
        crow
        AbsDatabase
        Controllers
)

target_link_libraries(execBinary
    PUBLIC
        sqlite3
        crow
        AbsDatabase
        Controllers
)
