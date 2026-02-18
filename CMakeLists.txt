cmake_minimum_required(VERSION 3.10)
project("CS1D_GROUP_PROJECT")

set(PROJECT_SOURCES src/main.cpp
        src/RouteRegister.h
        src/Controllers.h
        src/Controllers/UniversityController.cpp
        src/Controllers/UserController.cpp
        src/Controllers/AdminController.cpp
        src/Controllers/TourController.cpp
)

add_executable(execBinary ${PROJECT_SOURCES})

target_include_directories(execBinary PUBLIC ${CMAKE_SOURCE_DIR}/include/ASIO_headers
PUBLIC ${CMAKE_SOURCE_DIR}/include/crow_headers
)

