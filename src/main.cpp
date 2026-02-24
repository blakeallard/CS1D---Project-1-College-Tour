#include "Controllers.h"
#include "RouteRegister.h"
#include <crow.h>
#include <string>

int main()
{
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
            //res.code = 302;
            res.set_static_file_info("index.html");
            return res;
        });

    // Serve ALL other static files (js, css, etc.)
    CROW_ROUTE(app, "/<path>")
    (
        [](std::string path)
        {
            crow::response res;
            res.set_static_file_info(path);
            return res;
        });

    app.port(18080).multithreaded().run();
}
