/**
 * @file PurchaseController.cpp
 * @brief REST API controller for souvenir purchase operations
 * 
 * Handles HTTP requests for saving and retrieving souvenir
 * purchases made during campus tours. Provides data persistence
 * for purchase history.
 * 
 * @see Controllers.h
 */

#include "Controllers.h"
#include "Queries.h"
#include <iostream>

/**
 * @brief Handles GET requests for purchase history
 * 
 * Retrieves all purchase records for a specific user.
 * 
 * GET /api/Purchase/{userId}
 * 
 * Response:
 * @code
 * [
 *   {
 *     "id": 1,
 *     "campus": "UCLA",
 *     "item": "T-Shirt",
 *     "price": 25.99,
 *     "quantity": 2,
 *     "purchaseDate": "2026-03-08"
 *   },
 *   ...
 * ]
 * @endcode
 * 
 * @param id User ID as string (will be converted to integer)
 * @return JSON array of purchase records
 * 
 * Time Complexity: O(n) where n is number of purchases
 */
crow::response PurchaseController::read(std::string id)
{
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

/**
 * @brief Handles POST requests for saving purchases
 * 
 * Saves souvenir purchases made during a tour to the database.
 * Each purchase includes campus, item, price, and quantity.
 * 
 * POST /api/Purchase/save
 * 
 * Request Body:
 * @code
 * {
 *   "tourName": "Saddleback Tour",
 *   "purchases": [
 *     {"campus": "UCLA", "item": "T-Shirt", "price": 25.99, "quantity": 2},
 *     {"campus": "USC", "item": "Coffee Mug", "price": 12.50, "quantity": 1}
 *   ]
 * }
 * @endcode
 * 
 * Response:
 * @code
 * {
 *   "success": true,
 *   "message": "Purchases saved successfully"
 * }
 * @endcode
 * 
 * @param req HTTP request object containing purchase data
 * @param id Action identifier (must be "save")
 * @return JSON response with success status
 * 
 * @note Currently uses user_id = 1 (guest user)
 * @note Future: user_id should come from authentication
 * 
 * Time Complexity: O(n) where n is number of purchase items
 */
crow::response PurchaseController::create(const crow::request &req, std::string id)
{
    if (id == "save")
    {
        try
        {
            auto jsonData = crow::json::load(req.body);
            
            if (!jsonData)
            {
                return crow::response(400, "Invalid JSON");
            }
            
            // Guest user ID (future: from authentication)
            int userId = 1;
            
            std::string tourName = jsonData["tourName"].s();
            
            // Insert each purchase record
            if (jsonData.has("purchases"))
            {
                for (const auto &purchase : jsonData["purchases"])
                {
                    std::string campus = purchase["campus"].s();
                    std::string item = purchase["item"].s();
                    double price = purchase["price"].d();
                    int quantity = purchase["quantity"].i();
                    
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

/**
 * @brief Handles PATCH requests for purchase updates
 * 
 * Not implemented - purchases are immutable records.
 * 
 * @param req HTTP request object
 * @param id Purchase identifier
 * @return HTTP 501 Not Implemented
 */
crow::response PurchaseController::patch(const crow::request &req, std::string id)
{
    return crow::response(501, "Not implemented");
}

/**
 * @brief Handles DELETE requests for purchase removal
 * 
 * Not implemented - purchases are immutable records.
 * 
 * @param req HTTP request object
 * @param id Purchase identifier
 * @return HTTP 501 Not Implemented
 */
crow::response PurchaseController::remove(const crow::request &req, std::string id)
{
    return crow::response(501, "Not implemented");
}
