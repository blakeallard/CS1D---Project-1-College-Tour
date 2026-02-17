#include "../Controllers.h"
using namespace std;

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
    return result;
}
