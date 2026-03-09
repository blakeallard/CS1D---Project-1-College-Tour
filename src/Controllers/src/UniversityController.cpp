#include "crow/http_response.h"
#include <Controllers.h>
#include <DatabaseTypes.h>
#include <Helpers.h>
#include <Queries.h>
using namespace std;

crow::response UniversityController::read(std::string id)
{
    std::cout << "ID received: '" << id << "'" << std::endl;
    crow::json::wvalue result;

    if (id == "all")
    {
        QueryData::QueryResult campuses =
            QueryData::selectRows("distances.db", "colleges", {"college"});

        int i = 0;
        for (const auto &row : campuses)
        {
            string campusName             = get<string>(row.at("college"));
            result["campuses"][i]["name"] = campusName;

            QueryData::QueryResult distances =
                QueryData::selectRows("distances.db", "distances", {"distance"},
                                      {"starting_college", "ending_college"},
                                      {"Saddleback College", campusName});

            if (!distances.empty())
                result["campuses"][i]["distance"] =
                    std::get<double>(distances[0].at("distance"));
            else
                result["campuses"][i]["distance"] = -1;
            i++;
        }
    }
    else if (id.rfind("from-", 0) == 0)
    {
        // Handle "from-{campusName}" - return campuses reachable from specified campus
        string startCampus = id.substr(5); // Remove "from-" prefix

        QueryData::QueryResult campuses = QueryData::selectRows(
            "distances.db", "distances", {"ending_college", "distance"},
            {"starting_college"}, {startCampus});

        int i = 0;
        for (const auto &row : campuses)
        {
            string campusName = get<string>(row.at("ending_college"));
            double dist = get<double>(row.at("distance"));
            result["campuses"][i]["name"] = campusName;
            result["campuses"][i]["distance"] = dist;
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

    return crow::response(result);
}

crow::response UniversityController::patch(const crow::request &req,
                                           std::string id)
{
    crow::response result;
    return result;
}

crow::response UniversityController::remove(const crow::request &req,
                                            std::string id)
{
    // return success
    crow::response result;
    return result;
}

crow::response UniversityController::create(const crow::request &req,
                                            std::string id)
{
    try
    {
        if (id == "import")
        {
            Helpers::getDatabaseFromRequest(req);

            Helpers::mergeDatabases("Databases/distances.db",
                                    "Databases/uploaded.db", "distances",
                                    "distances");
            Helpers::getDatabaseFromRequest(req);
            Helpers::mergeDatabases(
                "Databases/distances.db", "Databases/uploaded.db", "colleges",
                "distances", true, {}, {}, "college", "starting_college");
        }
    }
    catch (std::runtime_error e)
    {
        std::cerr << e.what() << std::endl;
        return crow::response(400);
    }
    return crow::response(200, "");
}
