#include "../Controllers.h"

crow::json::wvalue UniversityController::read(std::string id)
{
    Database db;
    db.ConnectToDB("distances.db", "souvenirs.db");
    crow::json::wvalue result;

    if (id == "all")
    {
        std::vector<std::string> campuses = db.GetAllCampuses();
        for (int i = 0; i <(int)campuses.size(); i++)
        {
            result["campuses"][i]["name"] = campuses[i];
            result["campuses"][i]["distance"] = db.GetDistance("Saddleback College", campuses[i]);
        }
    }
    else
    {
        std::vector<Souvenir> souvenirs = db.GetSouvenirs(id);
        for (int i = 0; i < (int)souvenirs.size(); i++)
        {
            result["souvenirs"][i]["item"] = souvenirs[i].item;
            result["souvenirs"][i]["price"] = souvenirs[i].price;
        }
    }
    db.CloseDB();
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
