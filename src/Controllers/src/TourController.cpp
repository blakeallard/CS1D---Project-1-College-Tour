/**
 * @file TourController.cpp
 * @brief REST API controller for tour planning operations
 * 
 * Handles HTTP requests for calculating optimal tour routes
 * through college campuses. Uses TourPlanner for route optimization.
 * 
 * @see TourPlanner
 * @see Controllers.h
 */

#include <Controllers.h>
#include <TourPlanner.h>
#include <Queries.h>
#include <iostream>
using namespace std;

/**
 * @brief Handles GET requests for tour data
 * 
 * Currently returns a placeholder response. Can be extended
 * to retrieve saved tour information.
 * 
 * @param id Tour identifier or action
 * @return JSON response with tour data
 * 
 * Time Complexity: O(1)
 */
crow::response TourController::read(std::string id)
{
    crow::json::wvalue result;
    result["value"] = 1;
    return crow::response(result);
}

/**
 * @brief Handles PATCH requests for tour updates
 * 
 * Placeholder for future tour modification functionality.
 * 
 * @param req HTTP request object
 * @param id Tour identifier
 * @return Empty response
 */
crow::response TourController::patch(const crow::request &req, std::string id)
{
    crow::response result;
    return result;
}

/**
 * @brief Handles DELETE requests for tour removal
 * 
 * Placeholder for future tour deletion functionality.
 * 
 * @param req HTTP request object
 * @param id Tour identifier
 * @return Empty response
 */
crow::response TourController::remove(const crow::request &req, std::string id)
{
    crow::response result;
    return result;
}

/**
 * @brief Handles POST requests for tour calculations
 * 
 * Calculates optimal tour routes based on starting campus
 * and selected campuses to visit. Uses greedy nearest-neighbor
 * algorithm with 2-opt optimization.
 * 
 * Endpoints:
 * - POST /api/Tour/calculate - Calculate tour with user-selected campuses
 * - POST /api/Tour/custom - Alias for calculate
 * 
 * Request Body:
 * @code
 * {
 *   "startCampus": "Saddleback College",
 *   "selectedCampuses": ["UCLA", "USC", "Arizona State University"]
 * }
 * @endcode
 * 
 * Response:
 * @code
 * {
 *   "success": true,
 *   "totalDistance": 245.5,
 *   "campusCount": 4,
 *   "route": [
 *     {"campus": "Saddleback College", "distanceFromPrevious": 0},
 *     {"campus": "UCLA", "distanceFromPrevious": 30.2},
 *     ...
 *   ]
 * }
 * @endcode
 * 
 * @param req HTTP request object containing tour parameters
 * @param id Endpoint identifier ("calculate" or "custom")
 * @return JSON response with optimal route and total distance
 * 
 * @note Uses TourPlanner::calculateOptimalTour for route optimization
 * 
 * Time Complexity: O(n^2) where n = number of campuses
 * - Greedy nearest-neighbor: O(n^2)
 * - 2-opt optimization: O(n^2) per iteration
 * 
 * @see TourPlanner::calculateOptimalTour
 */
crow::response TourController::create(const crow::request &req, std::string id)
{
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

            vector<string> selectedCampuses;
            if (jsonData.has("selectedCampuses"))
            {
                for (const auto &campus : jsonData["selectedCampuses"])
                {
                    selectedCampuses.push_back(campus.s());
                }
            }

            TourResult tour = TourPlanner::calculateOptimalTour(startCampus, selectedCampuses);

            crow::json::wvalue response;
            response["success"] = true;
            response["totalDistance"] = tour.totalDistance;
            response["campusCount"] = (int)tour.stops.size();

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
    else if (id == "calculate-n")
    {
        /**
         * @brief Calculate tour visiting N nearest campuses
         * 
         * POST /api/Tour/calculate-n
         * Body: { startCampus: string, numberOfCampuses: int }
         * 
         * Automatically selects the N-1 nearest campuses from the starting
         * campus and calculates the optimal tour using greedy algorithm.
         * 
         * Time Complexity: O(n log n) for sorting + O(n^2) for tour calculation
         */
        try
        {
            auto jsonData = crow::json::load(req.body);

            if (!jsonData)
            {
                return crow::response(400, "Invalid JSON");
            }

            string startCampus = jsonData["startCampus"].s();
            int numberOfCampuses = jsonData["numberOfCampuses"].i();

            // Get all campuses reachable from startCampus with distances
            QueryData::QueryResult allDistances = QueryData::selectRows(
                "distances.db", "distances",
                {"ending_college", "distance"},
                {"starting_college"}, {startCampus});

            // Sort by distance and select N-1 nearest
            vector<pair<string, double>> campusDistances;
            for (const auto &row : allDistances)
            {
                string endCampus = get<string>(row.at("ending_college"));
                double dist = get<double>(row.at("distance"));
                campusDistances.push_back({endCampus, dist});
            }

            // Sort by distance ascending - O(n log n)
            sort(campusDistances.begin(), campusDistances.end(),
                 [](const pair<string, double> &a, const pair<string, double> &b) {
                     return a.second < b.second;
                 });

            // Select the N-1 nearest campuses (N includes the starting campus)
            vector<string> selectedCampuses;
            int campusesToSelect = min(numberOfCampuses - 1, (int)campusDistances.size());
            for (int i = 0; i < campusesToSelect; i++)
            {
                selectedCampuses.push_back(campusDistances[i].first);
            }

            // Calculate optimal tour through selected campuses - O(n^2)
            TourResult tour = TourPlanner::calculateOptimalTour(startCampus, selectedCampuses);

            crow::json::wvalue response;
            response["success"] = true;
            response["totalDistance"] = tour.totalDistance;
            response["campusCount"] = (int)tour.stops.size();

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
            cerr << "Error calculating N-campus tour: " << e.what() << endl;
            return crow::response(500, "Internal server error");
        }
    }

    crow::response result;
    return result;
}
