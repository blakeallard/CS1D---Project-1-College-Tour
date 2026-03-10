#ifndef TOUR_PLANNER_H
#define TOUR_PLANNER_H

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief Represents a stop on a tour
 */
struct TourStop
{
        std::string campus;
        double distanceFromPrevious;
};

/**
 * @brief Represents a complete tour itinerary
 */
struct TourResult
{
        std::vector<TourStop> stops;
        double totalDistance;
};

class TourPlanner
{
    private:
        /**
         * @brief Helper function to get all distances from a campus
         *
         * @param campus Starting campus
         * @param allCampuses List of all available campuses
         * @return Map of campus names to distances
         */
        static std::unordered_map<std::string, double>
        getDistancesFromCampus(const std::string &campus,
                               const std::vector<std::string> &allCampuses);

    public:
        static TourResult
        calculateOptimalTour(const std::string &startCampus,
                             const std::vector<std::string> &campusesToVisit);
};

#endif
