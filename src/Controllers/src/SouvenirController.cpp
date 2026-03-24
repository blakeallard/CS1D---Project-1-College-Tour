/**
 * @file SouvenirController.cpp
 * @brief REST API controller for souvenir operations
 * 
 * Handles HTTP requests for retrieving, creating, updating,
 * and deleting souvenirs in the souvenirs database.
 * 
 * @see Controllers.h
 */

#include "Controllers.h"
#include "Queries.h"
#include "crow/json.h"
#include <Helpers.h>
#include <iostream>
#include <stdexcept>

/**
 * @brief Handles GET requests for souvenir data
 * 
 * Retrieves all souvenirs for a specific campus.
 * 
 * GET /api/Souvenir/{campusName}
 * 
 * Response:
 * @code
 * [
 *   {"name": "T-Shirt", "price": 25.99},
 *   {"name": "Coffee Mug", "price": 12.50},
 *   ...
 * ]
 * @endcode
 * 
 * @param id Campus name to retrieve souvenirs for
 * @return JSON array of souvenirs with names and prices
 * 
 * Time Complexity: O(n) where n is number of souvenirs for the campus
 */
crow::response SouvenirController::read(std::string id)
{
    crow::json::wvalue result;
    try
    {
        QueryData::QueryResult souvenirs = QueryData::selectRows(
            "souvenirs.db", "souvenirs", {"item", "price"}, {"college"}, {id});

        int i = 0;
        for (const auto &row : souvenirs)
        {

            crow::json::wvalue item;
            item["name"]  = std::get<std::string>(row.at("item"));
            item["price"] = std::get<double>(row.at("price"));

            result[i] = std::move(item);
            i++;
        }
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
        return crow::response(400);
    }

    return crow::response(result);
}

/**
 * @brief Handles PATCH requests for souvenir updates
 * 
 * Updates a souvenir's name and/or price for a specific campus.
 * 
 * PATCH /api/Souvenir/{campusName}
 * 
 * Request Body:
 * @code
 * {
 *   "itemName": "Old T-Shirt",
 *   "newName": "Premium T-Shirt",
 *   "price": 29.99
 * }
 * @endcode
 * 
 * @param req HTTP request object containing update data
 * @param id Campus name where the souvenir belongs
 * @return HTTP 200 on success, HTTP 400 on error
 * 
 * @note Requires administrator authentication (handled by frontend)
 */
crow::response SouvenirController::patch(const crow::request &req,
                                         std::string id)
{
    auto body = crow::json::load(req.body);

    if (!body)
        return crow::response(400);

    try
    {
        std::string itemName = body["itemName"].s();
        std::string newName  = body["newName"].s();
        double price         = body["price"].d();

        QueryData::updateRows("souvenirs.db", "souvenirs", {"college", "item"},
                              {id, itemName}, {"item", "price"},
                              {newName, price});
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
        return crow::response(400);
    }

    return crow::response(200);
}

/**
 * @brief Handles POST requests for creating souvenirs
 * 
 * Supports two operations:
 * 
 * 1. Import souvenirs from file:
 *    POST /api/Souvenir/import
 *    - Imports souvenir data from uploaded database file
 *    - Merges with existing souvenirs.db
 * 
 * 2. Add single souvenir:
 *    POST /api/Souvenir/{campusName}
 *    Body: { "name": "New Item", "price": 15.99 }
 *    - Creates new souvenir for specified campus
 * 
 * @param req HTTP request object containing souvenir data
 * @param id Action identifier ("import") or campus name
 * @return HTTP 200 on success, HTTP 400 on error
 * 
 * @note Requires administrator authentication for import
 */
crow::response SouvenirController::create(const crow::request &req,
                                          std::string id)
{
    auto body = crow::json::load(req.body);

    try
    {
        if (id == "import")
        {
            // Import souvenirs from uploaded file
            Helpers::getDatabaseFromRequest(req);

            Helpers::mergeDatabases("Databases/souvenirs.db",
                                    "Databases/uploaded.db", "souvenirs",
                                    "souvenirs", false, {}, {}, "item", "item");
        }
        else
        {
            // Add single souvenir for campus
            std::string newName = body["name"].s();
            double newPrice     = body["price"].d();

            QueryData::insertRow("souvenirs.db", "souvenirs",
                                 {"college", "item", "price"},
                                 {id, newName, newPrice});
        }
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
        return crow::response(400);
    }

    return crow::response(200);
}

/**
 * @brief Handles DELETE requests for removing souvenirs
 * 
 * Deletes a specific souvenir from a campus.
 * 
 * DELETE /api/Souvenir/{campusName}
 * Body: { "item": "Item Name" }
 * 
 * @param req HTTP request object with item to delete
 * @param id Campus name where the souvenir belongs
 * @return HTTP 200 on success, HTTP 400 on error
 * 
 * @note Requires administrator authentication (handled by frontend)
 */
crow::response SouvenirController::remove(const crow::request &req,
                                          std::string id)
{
    crow::json::wvalue result;
    auto body = crow::json::load(req.body);
    if (!body)
        return crow::response(400);
    try
    {
        std::string itemName = body["item"].s();

        QueryData::deleteRow("souvenirs.db", "souvenirs", {"college", "item"},
                             {id, itemName});
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
        return crow::response(400);
    }

    return crow::response(200);
}
