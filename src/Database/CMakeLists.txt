
set(ABS_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AbsDatabase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Queries.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/TourPlanner.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/AbsDatabase.h
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/Queries.h
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/TourPlanner.h
        ${CMAKE_CURRENT_SOURCE_DIR}/headers/LinkedHeapTree.h
)

add_library(AbsDatabase STATIC ${ABS_SOURCES})

target_include_directories(AbsDatabase
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(AbsDatabase
    PUBLIC
    sqlite3
)
