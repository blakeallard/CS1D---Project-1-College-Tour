#include <Controllers.h>
#include <DatabaseTypes.h>
#include <Queries.h>
#include <stdexcept>
using namespace std;

crow::json::wvalue UniversityController::read(std::string id)
{
    std::cout << "ID received: '" << id << "'" << std::endl;
    crow::json::wvalue result;

    if (id == "all")
    {

        QueryData::QueryResult campusQuery = QueryData::selectRowsWithQuery(
            "souvenirs.db",
            "SELECT DISTINCT college FROM souvenirs ORDER BY college");

        int i = 0;
        for (const auto &row : campusQuery)
        {
            std::string campusName = std::get<std::string>(row.at("college"));
            result["campuses"][i]["name"] = campusName;

            QueryData::QueryResult distanceQuery =
                QueryData::selectRows("distances.db", "distances", {"distance"},
                                      {"starting_college", "ending_college"},
                                      {"Saddleback College", campusName});

            // If not using an iterator must check for empty
            if (!distanceQuery.empty())
                result["campuses"][i]["distance"] =
                    std::get<double>(distanceQuery[0].at("distance"));
            else
                result["campuses"][i]["distance"] = -1;
            i++;
        }
    }
    else
    {

        QueryData::QueryResult souvenirs = QueryData::selectRows(
            "souvenirs.db", "souvenirs", {"item", "price"}, {"college"}, {id});

        int i = 0;
        cout << "SIZE: " << souvenirs.size();
        for (const auto &row : souvenirs)
        {
            result["souvenirs"][i]["item"]  = get<string>(row.at("item"));
            result["souvenirs"][i]["price"] = get<double>(row.at("price"));
            i++;
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
