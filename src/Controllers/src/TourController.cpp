#include <Controllers.h>
#include <TourPlanner.h>
#include <Queries.h>
#include <iostream>
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
    // POST /api/Tour/calculate - Calculate optimal tour route
    if (id == "calculate" || id == "custom")
    {
        try
        {
            auto jsonData = crow::json::load(req.body);

            if (!jsonData)
            {
                return crow::response(400, "Invalid JSON");
            }

            string startCampus = jsonData["startCampus"].s();

            // Get selected campuses array
            vector<string> selectedCampuses;
            if (jsonData.has("selectedCampuses"))
            {
                for (const auto &campus : jsonData["selectedCampuses"])
                {
                    selectedCampuses.push_back(campus.s());
                }
            }

            // Calculate optimal tour using recursive backtracking
            TourResult tour = TourPlanner::calculateOptimalTour(startCampus, selectedCampuses);

            // Build JSON response
            crow::json::wvalue response;
            response["success"] = true;
            response["totalDistance"] = tour.totalDistance;
            response["campusCount"] = (int)tour.stops.size();

            // Add route stops
            int i = 0;
            for (const auto &stop : tour.stops)
            {
                response["route"][i]["campus"] = stop.campus;
                response["route"][i]["distanceFromPrevious"] = stop.distanceFromPrevious;
                i++;
            }

            return crow::response(200, response);
        }
        catch (const exception &e)
        {
            cerr << "Error calculating tour: " << e.what() << endl;
            return crow::response(500, "Internal server error");
        }
    }

    crow::response result;
    return result;
}
