/**
 * @file UserController.cpp
 * @brief REST API controller for user operations
 * 
 * Handles HTTP requests for user management including
 * authentication, purchase summaries, and wallet operations.
 * Also provides admin login functionality.
 * 
 * @see Controllers.h
 */

#include "Queries.h"
#include <Controllers.h>
#include <sstream>
using namespace std;

/**
 * @brief Handles GET requests for user data
 * 
 * Retrieves user-related data such as purchase summaries.
 * Currently returns empty response (TODO: implement).
 * 
 * Future functionality:
 * - "summary": Get user's balance, total spent, and all purchases
 * - "{campusId}": Get purchases and spending for specific campus
 * 
 * @param id Query type ("summary") or campus ID
 * @return JSON response with user data
 */
crow::response UserController::read(std::string id)
{
    crow::json::wvalue result;

    /* TODO refactor
    if (id == "summary")
    {
        result["balances"] = wallet.GetBalance();
        result["totalSpent"] = wallet.GetTotalSpent();

        std::vector<PurchasedItem> items = purchases.GetAllItems();
        for (int i = 0; i < (int)items.size(); i++)
        {
            result["items"][i]["campus"] = items[i].campus;
            result["items"][i]["item"] = items[i].item;
            result["items"][i]["price"] = items[i].price;
            result["items"][i]["quantity"] = items[i].quantity;
        }
    }
    else
    {
        std::vector<PurchasedItem> items = purchases.GetItemsByCampus(id);
        for (int i = 0; i < (int)items.size(); i++)
        {
            result["items"][i]["item"] = items[i].item;
            result["items"][i]["price"] = items[i].price;
            result["items"][i]["quantity"] = items[i].quantity;
        }
        result["campusSpent"] = wallet.GetSpendingByCampus(id);
    }
    */

    return crow::response(result);
}

/**
 * @brief Handles PATCH requests for user updates
 * 
 * Future functionality: Add funds to user wallet.
 * 
 * @param req HTTP request object
 * @param id Amount to add (as string)
 * @return JSON response with updated balance
 */
crow::response UserController::patch(const crow::request &req, std::string id)
{
    crow::response result;

    /* TODO refactor
    double amount = std::stod(id);
    wallet.AddFunds(amount);
    result["balance"] = wallet.GetBalance();
    */

    return result;
}

/**
 * @brief Handles DELETE requests for user removal
 * 
 * Placeholder for future user deletion functionality.
 * 
 * @param req HTTP request object
 * @param id User identifier
 * @return Empty response
 */
crow::response UserController::remove(const crow::request &req, std::string id)
{
    crow::response result;
    return result;
}

/**
 * @brief Handles POST requests for user actions
 * 
 * Supports administrator login:
 * 
 * POST /api/User/admin
 * Request Body:
 * @code
 * {
 *   "username": "admin",
 *   "password": "password123"
 * }
 * @endcode
 * 
 * Success Response (HTTP 200):
 * @code
 * {
 *   "token": "admin-session-token"
 * }
 * @endcode
 * 
 * Failure Response: HTTP 401 Unauthorized
 * 
 * @param req HTTP request object containing credentials
 * @param id Action identifier ("admin" for login)
 * @return JSON response with session token or 401 error
 * 
 * @note Password is compared in plaintext (production should use hashing)
 * 
 * Time Complexity: O(1) for single admin lookup
 */
crow::response UserController::create(const crow::request &req, std::string id)
{

    auto body = crow::json::load(req.body);

    if (id == "admin")
    {
        // Query admin credentials from database
        QueryData::QueryResult credentials =
            QueryData::selectRows("users.db", "users", {"username", "password"},
                                  {"username"}, {"admin"});

        string username = body["username"].s();
        string password = body["password"].s();
        string dbUsername;
        string dbPassword;
        if (!credentials.empty())
        {

            dbUsername = get<string>(credentials[0].at("username"));
            dbPassword = get<string>(credentials[0].at("password"));
        }

        // Validate credentials
        if (username == dbUsername && password == dbPassword)
        {
            crow::json::wvalue res;
            res["token"] = "admin-session-token";
            return crow::response(res);
        }
        else
        {
            return crow::response(401);
        }
    }

    return crow::response(200);
}
