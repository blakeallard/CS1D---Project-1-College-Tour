#ifndef ROUTE_REGISTER_H
#define ROUTE_REGISTER_H
#include <crow.h>
#include <string>

/**
 * @brief Decodes a URL-encoded string (percent-encoding).
 * * Scans the input string for '%' characters followed by two hexadecimal
 * digits and converts them back into their original ASCII character
 * representation.
 * * @param id The URL-encoded string to be processed.
 * @return A new std::string containing the decoded plain-text data.
 * @note If a '%' is not followed by two valid hex digits, the characters are
 * treated as literal text.
 */
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

/**
 * @brief Automatically registers CRUD API endpoints for a given controller.
 * * Maps standard HTTP methods to static methods within the provided Controller
 * class. All routes are prefixed with `/api/` and include a trailing dynamic
 * string ID.
 * *
 * * ### Controller Requirements
 * The `Controller` template type must implement the following static methods:
 * - `static crow::response create(const crow::request&, std::string)`
 * - `static crow::response read(std::string)`
 * - `static crow::response patch(const crow::request&, std::string)`
 * - `static crow::response remove(const crow::request&, std::string)`
 * * ### Endpoint Mapping
 * | Method | Action  | Controller Method |
 * | :----- | :------ | :---------------- |
 * | GET    | Read    | Controller::read   |
 * | POST   | Create  | Controller::create |
 * | PATCH  | Update  | Controller::patch  |
 * | DELETE | Delete  | Controller::remove |
 * * @tparam Controller A class/struct defining the logic for each HTTP action.
 * @param app Reference to the Crow application instance.
 * @param basePath The resource name used in the URL (e.g., "users/").
 */
template <typename Controller>
void register_crud_routes(crow::SimpleApp &app, const std::string &basePath)
{

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
