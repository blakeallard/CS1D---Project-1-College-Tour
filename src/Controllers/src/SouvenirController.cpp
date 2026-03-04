#include "Controllers.h"
#include "Queries.h"
#include "crow/json.h"
#include <iostream>
#include <stdexcept>

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

crow::response SouvenirController::create(const crow::request &req,
                                          std::string id)
{
    auto body = crow::json::load(req.body);
    if (!body)
        return crow::response(400);

    try
    {
        std::string newName = body["name"].s();
        double newPrice     = body["price"].d();

        QueryData::insertRow("souvenirs.db", "souvenirs",
                             {"college", "item", "price"},
                             {id, newName, newPrice});
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
        return crow::response(400);
    }

    return crow::response(200);
}

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
