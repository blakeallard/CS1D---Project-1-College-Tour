#include "../Controllers.h"
#include <sstream>
using namespace std;

Wallet UserController::wallet;
StudentPurchases UserController::purchases;

crow::json::wvalue UserController::read(std::string id)
{
    // Get value, then store it in a json then return the json
    crow::json::wvalue result;
    result["value"] = 1;
    return result;
}

crow::json::wvalue UserController::patch(std::string id)
{
    crow::json::wvalue result;
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

