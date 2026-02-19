cmake_minimum_required(VERSION 3.10)
project("CS1D_GROUP_PROJECT")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(vendor/sqlite3)

get_target_property(SQLITE_INCLUDES sqlite3 INCLUDE_DIRECTORIES)
message(STATUS "SQLite includes: ${SQLITE_INCLUDES}")

set(PROJECT_SOURCES src/main.cpp
        src/RouteRegister.h
        src/Controllers.h
        src/Controllers/UniversityController.cpp
        src/Controllers/UserController.cpp
        src/Controllers/AdminController.cpp
        src/Controllers/TourController.cpp
        src/database.cpp
        src/Wallet.cpp
        src/StudentPurchases.cpp
)

add_executable(execBinary ${PROJECT_SOURCES})

target_include_directories(execBinary
    PUBLIC sqlite3
    PUBLIC ${CMAKE_SOURCE_DIR}/include/ASIO_headers
    PUBLIC ${CMAKE_SOURCE_DIR}/include/crow_headers
)

target_link_libraries(execBinary
    PRIVATE
        sqlite3
)
