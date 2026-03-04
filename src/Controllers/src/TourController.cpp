#include <Controllers.h>
using namespace std;

crow::response TourController::read(std::string id)
{
    // Get value, then store it in a json then return the json
    crow::json::wvalue result;
    result["value"] = 1;
    return crow::response(result);
}

crow::response TourController::patch(const crow::request &req, std::string id)
{
    crow::response result;
    return result;
}

crow::response TourController::remove(const crow::request &req, std::string id)
{
    // return success
    crow::response result;
    return result;
}

crow::response TourController::create(const crow::request &req, std::string id)
{
    crow::response result;
    return result;
}
