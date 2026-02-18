#include "Controllers.h"
#include "RouteRegister.h"
#include <crow.h>
#include <string>

int main()
{
    crow::SimpleApp app;
    CROW_ROUTE(app, "/")(
        []()
        {
            crow::response res;
            res.code = 302;
            res.set_header("Location", "/static/index.html");
            return res;
        });

    register_crud_routes<UniversityController>(app, "University/");
    register_crud_routes<UserController>(app, "User/");
    register_crud_routes<AdminController>(app, "Admin/");
    register_crud_routes<TourController>(app, "Tour/");

    app.port(18080).multithreaded().run();
}
