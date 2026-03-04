#include <Controllers.h>
#include <DatabaseTypes.h>
#include <Queries.h>
using namespace std;

crow::json::wvalue UniversityController::read(std::string id)
{
    std::cout << "ID received: '" << id << "'" << std::endl;
    crow::json::wvalue result;

    if (id == "all")
    {

        QueryData::QueryResult campuses = QueryData::selectRowsWithQuery(
            "souvenirs.db",
            "SELECT DISTINCT college FROM souvenirs ORDER BY college");

        int i = 0;
        // For rows in the campuses return
        for (const auto &row : campuses)
        {
            // We get the campus name by specifying the row.at(column) then
            // converting it to a string
            string campusName             = get<string>(row.at("college"));
            result["campuses"][i]["name"] = campusName;

            QueryData::QueryResult distances =
                // Get distances, where starting and ending college are
                // saddleback and the campus we're iterating
                QueryData::selectRows("distances.db", "distances", {"distance"},
                                      {"starting_college", "ending_college"},
                                      {"Saddleback College", campusName});

            // If not using an iterator must check for empty
            if (!distances.empty())
                result["campuses"][i]["distance"] =
                    std::get<double>(distances[0].at("distance"));
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
