#include <Controllers.h>
using namespace std;

crow::response AdminController::read(std::string id)
{
    // Get value, then store it in a json then return the json
    crow::json::wvalue result;
    return crow::response(result);
}

crow::response AdminController::patch(const crow::request &req, std::string id)
{
    crow::response result;
    return result;
}

crow::response AdminController::remove(const crow::request &req, std::string id)
{
    // return success
    crow::response result;
    return result;
}

crow::response AdminController::create(const crow::request &req, std::string id)
{
    crow::response result;
    return result;
}
