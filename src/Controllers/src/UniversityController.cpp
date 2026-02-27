#include <Controllers.h>
#include <DatabaseTypes.h>
#include <Queries.h>

crow::json::wvalue UniversityController::read(std::string id)
{
    std::cout << "ID received: '" << id << "'" << std::endl;
    crow::json::wvalue result;

    if (id == "all")
    {
        std::vector<std::string> campuses;
        QueryData::allCampuses(campuses);
        for (int i = 0; i < (int)campuses.size(); i++)
        {
            result["campuses"][i]["name"] = campuses[i];
            result["campuses"][i]["distance"] =
                QueryData::distance("Saddleback College", campuses[i]);
        }
    }
    else
    {
        std::vector<SouvenirItem> souvenirs;
        QueryData::souvenirs(id, souvenirs);

        for (int i = 0; i < (int)souvenirs.size(); i++)
        {
            result["souvenirs"][i]["item"]  = souvenirs[i].item;
            result["souvenirs"][i]["price"] = souvenirs[i].price;
        }
    }
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
