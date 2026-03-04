#ifndef ROUTE_REGISTER_H
#define ROUTE_REGISTER_H
#include <crow.h>
#include <string>

std::string decodeURL(std::string id)
{
    std::string decoded;
    for (size_t i = 0; i < id.size(); i++)
    {
        if (id[i] == '%' && i + 2 < id.size())
        {
            int hex = std::stoi(id.substr(i + 1, 2), nullptr, 16);
            decoded += (char)hex;
            i += 2;
        }
        else
            decoded += id[i];
    }
    return decoded;
}

template <typename Controller>
void register_crud_routes(crow::SimpleApp &app, const std::string &basePath)
{

    /* Create API path, then set same path different method, then set the
     * lambda for the controller template to return a json to the caller */

    app.route_dynamic("/api/" + basePath + "<string>")
        .methods(crow::HTTPMethod::GET)(
            [](std::string id)
            {
                std::string url = decodeURL(id);
                return Controller::read(url);
            });

    app.route_dynamic("/api/" + basePath + "<string>")
        .methods(crow::HTTPMethod::POST)(
            [](const crow::request &req, std::string id)
            {
                std::string url = decodeURL(id);
                return Controller::create(req, url);
            });

    app.route_dynamic("/api/" + basePath + "<string>")
        .methods(crow::HTTPMethod::Patch)(
            [](const crow::request &req, std::string id)
            {
                std::string url = decodeURL(id);
                return Controller::patch(req, url);
            });

    app.route_dynamic("/api/" + basePath + "<string>")
        .methods("DELETE"_method)( // MSVC Oddity does not allow
                                   // ::DELETE To be taken
            [](const crow::request &req, std::string id)
            {
                std::string url = decodeURL(id);
                return Controller::remove(req, url);
            });
}

#endif

/*
 * Different Methods with the same path
| Method | What It Does
| ------ | --------------
| GET    | Read data
| POST   | Create data
| PUT    | Replace -- will not use for now
| PATCH  | Partial update
| DELETE | Delete
*/
