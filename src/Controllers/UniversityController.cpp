#include "../Controllers.h"
using namespace std;

crow::json::wvalue UniversityController::read(std::string id)
{
    // Get value, then store it in a json then return the json
    crow::json::wvalue result;
    result["value"] = 1;
    return result;
}

crow::json::wvalue UniversityController::patch(std::string id)
{
    crow::json::wvalue result;
    return result;
}

bool UniversityController::remove(std::string id)
{
    // return success
    return 1;
}

crow::json::wvalue UniversityController::create(std::string id)
{
    crow::json::wvalue result;
    return result;
}
