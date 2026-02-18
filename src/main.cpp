#include "Controllers.h"
#include "RouteRegister.h"
#include <crow.h>
#include <string>

int main()
{
    crow::SimpleApp app;
    // Serve index at /
    CROW_ROUTE(app, "/")(
        []()
        {
            crow::response res;
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
            res.set_static_file_info("frontend/dist/" + path);
            return res;
        });

    register_crud_routes<UniversityController>(app, "University/");
    register_crud_routes<UserController>(app, "User/");
    register_crud_routes<AdminController>(app, "Admin/");
    register_crud_routes<TourController>(app, "Tour/");

    app.port(18080).multithreaded().run();
}
