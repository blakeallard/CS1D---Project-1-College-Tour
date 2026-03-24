
set(CONT_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AdminController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TourController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/UniversityController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/UserController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/SouvenirController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/PurchaseController.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Helpers.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/headers/Controllers.h
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/Helpers.h
)

add_library(Controllers STATIC ${CONT_SOURCES})

target_include_directories(Controllers
    PUBLIC
        crow
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(Controllers
    PUBLIC
    crow
    sqlite3
    AbsDatabase
)
