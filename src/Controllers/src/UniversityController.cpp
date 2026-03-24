/**
 * @file UniversityController.cpp
 * @brief REST API controller for university/campus operations
 * 
 * Handles HTTP requests for retrieving campus information,
 * distances between campuses, and souvenir data. Also handles
 * administrative data imports.
 * 
 * @see Controllers.h
 */

#include "crow/http_response.h"
#include <Controllers.h>
#include <Helpers.h>
#include <Queries.h>
using namespace std;

/**
 * @brief Handles GET requests for university/campus data
 * 
 * Supports multiple query types based on the id parameter:
 * 
 * 1. "all" - Returns all campuses with distances from Saddleback College
 *    GET /api/University/all
 *    Response: { campuses: [{name, distance}, ...] }
 * 
 * 2. "from-{campusName}" - Returns campuses reachable from specified campus
 *    GET /api/University/from-Arizona%20State%20University
 *    Response: { campuses: [{name, distance}, ...] }
 * 
 * 3. "{campusName}" - Returns souvenirs for a specific campus
 *    GET /api/University/UCLA
 *    Response: { souvenirs: [{item, price}, ...] }
 * 
 * @param id Query type or campus name
 * @return JSON response with campus or souvenir data
 * 
 * Time Complexity: O(n) where n is number of results
 * - Database query: O(n)
 * - JSON construction: O(n)
 */
crow::response UniversityController::read(std::string id)
{
    std::cout << "ID received: '" << id << "'" << std::endl;
    crow::json::wvalue result;

    if (id == "all")
    {
        // Retrieve all campuses from the colleges table
        QueryData::QueryResult campuses =
            QueryData::selectRows("distances.db", "colleges", {"college"});

        int i = 0;
        for (const auto &row : campuses)
        {
            string campusName             = get<string>(row.at("college"));
            result["campuses"][i]["name"] = campusName;

            // Get distance from Saddleback College to this campus
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
        // Return campuses reachable from specified starting campus
        string startCampus = id.substr(5);

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
        // Return souvenirs for a specific campus
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

/**
 * @brief Handles PATCH requests for university data updates
 * 
 * Placeholder for future update functionality.
 * 
 * @param req HTTP request object
 * @param id Resource identifier
 * @return Empty response
 */
crow::response UniversityController::patch(const crow::request &req,
                                           std::string id)
{
    crow::response result;
    return result;
}

/**
 * @brief Handles DELETE requests for university data removal
 * 
 * Placeholder for future deletion functionality.
 * 
 * @param req HTTP request object
 * @param id Resource identifier
 * @return Empty response
 */
crow::response UniversityController::remove(const crow::request &req,
                                            std::string id)
{
    crow::response result;
    return result;
}

/**
 * @brief Handles POST requests for university data creation/import
 * 
 * Supports administrative data import functionality:
 * 
 * POST /api/University/import
 * - Imports new campus and distance data from uploaded file
 * - Merges data into existing distances.db and colleges table
 * - Used by administrators to add new campuses during runtime
 * 
 * @param req HTTP request object containing upload data
 * @param id Action identifier ("import")
 * @return HTTP 200 on success, HTTP 400 on error
 * 
 * @note Requires administrator authentication (handled by frontend)
 * 
 * @see Helpers::getDatabaseFromRequest
 * @see Helpers::mergeDatabases
 */
crow::response UniversityController::create(const crow::request &req,
                                            std::string id)
{
    try
    {
        if (id == "import")
        {
            // Extract uploaded database file from request
            Helpers::getDatabaseFromRequest(req);

            // Merge distances table
            Helpers::mergeDatabases("Databases/distances.db",
                                    "Databases/uploaded.db", "distances",
                                    "distances");
            
            // Merge colleges table (extracts unique colleges from distances)
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
