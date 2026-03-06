#include "Controllers.h"
#include "Queries.h"
#include <iostream>

crow::response PurchaseController::read(std::string id)
{
    // GET /api/Purchase/{userId} - Get user's purchase history
    try
    {
        int userId = std::stoi(id);
        
        QueryData::QueryResult purchases = QueryData::selectRows(
            "users.db", "user_purchases", {},
            {"user_id"}, {userId});
        
        crow::json::wvalue result;
        int i = 0;
        for (const auto &row : purchases)
        {
            result[i]["id"] = std::get<int>(row.at("id"));
            result[i]["campus"] = std::get<std::string>(row.at("campus"));
            result[i]["item"] = std::get<std::string>(row.at("item"));
            result[i]["price"] = std::get<double>(row.at("price"));
            result[i]["quantity"] = std::get<int>(row.at("quantity"));
            result[i]["purchaseDate"] = std::get<std::string>(row.at("purchase_date"));
            i++;
        }
        
        return crow::response(200, result);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading purchases: " << e.what() << std::endl;
        return crow::response(500, "Internal server error");
    }
}

crow::response PurchaseController::create(const crow::request &req, std::string id)
{
    // POST /api/Purchase/save - Save tour purchases
    if (id == "save")
    {
        try
        {
            auto jsonData = crow::json::load(req.body);
            
            if (!jsonData)
            {
                return crow::response(400, "Invalid JSON");
            }
            
            // For now, use user_id = 1 (guest user)
            // In a real app, this would come from authentication
            int userId = 1;
            
            std::string tourName = jsonData["tourName"].s();
            
            // Process each purchase item
            if (jsonData.has("purchases"))
            {
                for (const auto &purchase : jsonData["purchases"])
                {
                    std::string campus = purchase["campus"].s();
                    std::string item = purchase["item"].s();
                    double price = purchase["price"].d();
                    int quantity = purchase["quantity"].i();
                    
                    // Insert purchase record
                    QueryData::insertRow(
                        "users.db",
                        "user_purchases",
                        {"user_id", "campus", "item", "price", "quantity"},
                        {userId, campus, item, price, quantity}
                    );
                }
            }
            
            crow::json::wvalue response;
            response["success"] = true;
            response["message"] = "Purchases saved successfully";
            
            return crow::response(200, response);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error saving purchases: " << e.what() << std::endl;
            return crow::response(500, "Internal server error");
        }
    }
    
    return crow::response(400, "Invalid endpoint");
}

crow::response PurchaseController::patch(const crow::request &req, std::string id)
{
    return crow::response(501, "Not implemented");
}

crow::response PurchaseController::remove(const crow::request &req, std::string id)
{
    return crow::response(501, "Not implemented");
}
