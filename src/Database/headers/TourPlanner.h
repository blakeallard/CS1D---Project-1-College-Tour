#ifndef TOUR_PLANNER_H
#define TOUR_PLANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <limits>

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

/**
 * @brief Tour Planning Algorithm Implementation
 * 
 * This class implements the nearest-neighbor greedy algorithm for tour planning.
 * Time Complexity: O(n²) where n is the number of campuses to visit
 * 
 * The algorithm works by:
 * 1. Starting at the specified campus
 * 2. At each step, finding the nearest unvisited campus
 * 3. Moving to that campus and repeating until all campuses are visited
 */
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
    static std::unordered_map<std::string, double> getDistancesFromCampus(
        const std::string &campus,
        const std::vector<std::string> &allCampuses);

public:
    /**
     * @brief Calculate tour with campuses sorted by distance from start
     * 
     * SIMPLE ALGORITHM: Sort campuses by distance from starting campus only
     * - NOT greedy chaining
     * - NOT nearest-neighbor from current position
     * - Just: compute distances from start, sort ascending, build route
     * 
     * Steps:
     * 1. For each selected campus: distance = getDistance(start, campus)
     * 2. Sort campuses ascending by distance from start
     * 3. Build route in sorted order
     * 4. Calculate total distance as sum of consecutive hops
     * 
     * Time Complexity: O(n log n) for sorting
     * 
     * @param startCampus Fixed starting campus
     * @param selectedCampuses Vector of campuses user selected
     * @return TourResult with route sorted by distance from start
     */
    static TourResult calculateSortedByDistanceTour(
        const std::string &startCampus,
        const std::vector<std::string> &selectedCampuses);

    /**
     * @brief Calculate optimal tour using recursive backtracking
     * 
     * Big-Oh Analysis: O(n²)
     * - For each of n campuses to visit
     * - We search through remaining unvisited campuses to find nearest
     * - This gives us n * (n-1)/2 comparisons = O(n²)
     * 
     * @param startCampus Campus to start the tour from
     * @param campusesToVisit List of campuses to visit (excluding start)
     * @param enableLogging Enable detailed logging for algorithm verification (default: false)
     * @return TourResult containing ordered stops and total distance
     */
    static TourResult calculateOptimalTour(
        const std::string &startCampus,
        const std::vector<std::string> &campusesToVisit,
        bool enableLogging = false);

    /**
     * @brief Calculate tour visiting a specific number of nearest campuses
     * 
     * @param startCampus Campus to start from
     * @param numberOfCampuses How many campuses to visit
     * @param allCampuses Pool of available campuses
     * @return TourResult containing ordered stops and total distance
     */
    static TourResult calculateNearestNCampuses(
        const std::string &startCampus,
        int numberOfCampuses,
        const std::vector<std::string> &allCampuses);

    /**
     * @brief Calculate tour visiting all available campuses
     * 
     * @param startCampus Campus to start from
     * @param allCampuses All available campuses
     * @return TourResult containing ordered stops and total distance
     */
    static TourResult calculateFullTour(
        const std::string &startCampus,
        const std::vector<std::string> &allCampuses);
};

#endif
