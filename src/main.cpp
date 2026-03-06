#include "Controllers.h"
#include "Queries.h"
#include "RouteRegister.h"
#include <crow.h>
#include <stdexcept>
#include <string>

int main()
{

    crow::SimpleApp app;
    // Serve index at /

    register_crud_routes<UniversityController>(app, "University/");
    register_crud_routes<UserController>(app, "User/");
    register_crud_routes<AdminController>(app, "Admin/");
    register_crud_routes<TourController>(app, "Tour/");
    register_crud_routes<SouvenirController>(app, "Souvenir/");
    register_crud_routes<PurchaseController>(app, "Purchase/");

    CROW_ROUTE(app, "/")(
        []()
        {
            crow::response res;
            res.set_static_file_info("frontend/dist/index.html");
            return res;
        });

    // Serve ALL other static files (js, css, etc.)
    CROW_ROUTE(app, "/<path>")
    (
        [](std::string path)
        {
            crow::response res;

            std::string fullPath = "frontend/dist/" + path;

            // Try to serve the requested file
            res.set_static_file_info(fullPath);

            // If file not found, fallback to React index
            if (res.code == 404)
            {
                res.set_static_file_info("frontend/dist/index.html");
            }

            return res;
        });

    app.port(18080).multithreaded().run();
}
