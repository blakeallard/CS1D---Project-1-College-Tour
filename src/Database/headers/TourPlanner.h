#ifndef TOUR_PLANNER_H
#define TOUR_PLANNER_H

#include <string>
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
    public:
        static TourResult
        calculateOptimalTour(const std::string &startCampus,
                             const std::vector<std::string> &campusesToVisit);
};

#endif
