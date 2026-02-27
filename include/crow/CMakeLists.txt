add_library(crow INTERFACE)

target_include_directories(crow
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/ASIO_headers
        ${CMAKE_CURRENT_SOURCE_DIR}/crow_headers
)
