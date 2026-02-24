#include "../Controllers.h"
#include <sstream>
using namespace std;

Wallet UserController::wallet;
StudentPurchases UserController::purchases;

crow::json::wvalue UserController::read(std::string id)
{
    // Get value, then store it in a json then return the json
    crow::json::wvalue result;

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

    return result;
}

crow::json::wvalue UserController::patch(std::string id)
{
    crow::json::wvalue result;

    double amount = std::stod(id);
    wallet.AddFunds(amount);
    result["balance"] = wallet.GetBalance();

    return result;
}

bool UserController::remove(std::string id)
{
    // return success
    return 1;
}

crow::json::wvalue UserController::create(std::string id)
{
    crow::json::wvalue result;

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
        {
            decoded += id[i];
        }
    }
    id = decoded;

    std::stringstream ss(id);
    std::string campus;
    std::string item;
    std::string priceStr;
    std::string quantityStr;

    std::getline(ss, campus, ',');
    std::getline(ss, item, ',');
    std::getline(ss, priceStr, ',');
    std::getline(ss, quantityStr, ',');

    double price = std::stod(priceStr);
    int quantity = std::stoi(quantityStr);
    double total = price * quantity;

    if (wallet.DeductFunds(campus, total))
    {
        purchases.AddItem(campus, item, price, quantity);
        result["success"] = true;
        result["balances"] = wallet.GetBalance();
    }

    else
    {
        result["success"] = false;
        result["error"] = "Insufficient funds";
    }

    return result;
}

