#ifndef TOUR_PLANNER_H
#define TOUR_PLANNER_H

#include <map>
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
        // Calculates the optimal tour using a 2d matrix and a linked heap tree
        static TourResult
        calculateOptimalTour(const std::string &startCampus,
                             const std::vector<std::string> &campusesToVisit);

    protected:
        // Gets all distances from a campus and fills a 2d array
        static void getAllDistances(const std::vector<std::string> campusNames,
                                    std::vector<std::vector<double>> &matrix,
                                    std::map<std::string, int> &campusIndex);

        // simple greedy closest neighbour algorithm
        static void visit(const int current,
                          const std::vector<std::vector<double>> &distances,
                          std::vector<bool> &visited, std::vector<int> &route);

        // Simple short look ahead. Looks at every pair of distances and looks
        // at what it would be like if they swapped. If the distance is less,
        // then we swap for real. Works for local optimizations
        static void twoOpt(std::vector<int> &route,
                           const std::vector<std::vector<double>> &dist);
};

#endif
