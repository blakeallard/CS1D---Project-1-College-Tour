#include <Controllers.h>
using namespace std;

crow::json::wvalue AdminController::read(std::string id)
{
    // Get value, then store it in a json then return the json
    crow::json::wvalue result;
    result["value"] = 1;
    return result;
}

crow::json::wvalue AdminController::patch(std::string id)
{
    crow::json::wvalue result;
    return result;
}

bool AdminController::remove(std::string id)
{
    // return success
    return 1;
}

crow::json::wvalue AdminController::create(std::string id)
{
    crow::json::wvalue result;
    return result;
}
