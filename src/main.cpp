#include "Controllers.h"
#include "Queries.h"
#include "RouteRegister.h"
#include <crow.h>
#include <stdexcept>
#include <string>

int main()
{

    sqlite3 *db;

    if (sqlite3_open("Databases/Test.db", &db))
    {
        std::cerr << "Can't open database\n";
        return 1;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS testTable ("
                      "testColumn TEXT PRIMARY KEY,"
                      "testTwoColumn TEXT"
                      ");";

    char *errMsg = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    QueryData::insertRow("Test.db", "testTable",
                         {"testColumn", "testTwoColumn"},
                         {"ten", "4"});
    return 0;

    crow::SimpleApp app;
    // Serve index at /

    register_crud_routes<UniversityController>(app, "University/");
    register_crud_routes<UserController>(app, "User/");
    register_crud_routes<AdminController>(app, "Admin/");
    register_crud_routes<TourController>(app, "Tour/");

    CROW_ROUTE(app, "/")(
        []()
        {
            crow::response res;
            // res.code = 302;
            res.set_static_file_info("index.html");
            res.code = 302;
            res.set_static_file_info("frontend/dist/index.html");
            return res;
        });

    // Serve ALL other static files (js, css, etc.)
    CROW_ROUTE(app, "/<path>")
    (
        [](std::string path)
        {
            crow::response res;
            res.set_static_file_info(path);
            res.set_static_file_info("frontend/dist/" + path);
            return res;
        });

    app.port(18080).multithreaded().run();
}
