#include "Queries.h"
#include <Controllers.h>
#include <sstream>
using namespace std;

crow::response UserController::read(std::string id)
{
    // Get value, then store it in a json then return the json
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

crow::response UserController::remove(const crow::request &req, std::string id)
{
    crow::response result;
    // return success
    return result;
}

crow::response UserController::create(const crow::request &req, std::string id)
{

    auto body = crow::json::load(req.body);

    if (id == "admin")
    {
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
