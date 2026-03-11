/**
 * @file TourPlanner.h
 * @brief Tour planning algorithms for college campus visits
 * 
 * This file contains the TourPlanner class which implements algorithms
 * for finding efficient tour routes through college campuses using
 * greedy nearest-neighbor approach with 2-opt local optimization.
 * 
 * @author CS1D Group Project
 * @date March 2026
 */

#ifndef TOUR_PLANNER_H
#define TOUR_PLANNER_H

#include <map>
#include <string>
#include <vector>

/**
 * @brief Represents a single stop on a campus tour
 * 
 * Contains information about a campus visit including the campus name
 * and the distance traveled from the previous stop to reach it.
 */
struct TourStop
{
    std::string campus;           ///< Name of the campus at this stop
    double distanceFromPrevious;  ///< Distance in miles from previous stop
};

/**
 * @brief Represents a complete tour itinerary with route and total distance
 * 
 * Contains the ordered list of stops and the cumulative distance
 * traveled for the entire tour.
 */
struct TourResult
{
    std::vector<TourStop> stops;  ///< Ordered list of campus stops
    double totalDistance;         ///< Total distance of the tour in miles
};

/**
 * @class TourPlanner
 * @brief Calculates optimal tour routes through college campuses
 * 
 * This class provides static methods for computing efficient tour paths
 * using a combination of greedy nearest-neighbor algorithm and 2-opt
 * local optimization. The algorithm works as follows:
 * 
 * 1. Build a distance matrix from database queries
 * 2. Use greedy nearest-neighbor to build initial route (O(n²))
 * 3. Apply 2-opt optimization to improve route locally (O(n²) per iteration)
 * 
 * @note Uses LinkedHeapTree for efficient minimum distance selection
 * 
 * Data Structures Used:
 * - vector<vector<double>>: 2D distance matrix (adjacency matrix)
 * - map<string, int>: Campus name to index mapping
 * - LinkedHeapTree: Min-heap for nearest neighbor selection
 * - vector<bool>: Visited campus tracking
 * - vector<int>: Route as sequence of campus indices
 */
class TourPlanner
{
private:

public:
    /**
     * @brief Calculates the optimal tour visiting specified campuses
     * 
     * Main entry point for tour calculation. Builds a distance matrix,
     * computes a greedy nearest-neighbor route, then optimizes with 2-opt.
     * 
     * @param startCampus Name of the starting campus
     * @param campusesToVisit Vector of campus names to visit (excluding start)
     * @return TourResult containing the ordered stops and total distance
     * 
     * @note The starting campus is always the first stop in the result
     * 
     * Time Complexity: O(n²) where n = number of campuses
     * - Distance matrix construction: O(n) database queries
     * - Greedy nearest-neighbor: O(n²) using heap for each campus
     * - 2-opt optimization: O(n²) per iteration, typically few iterations
     * 
     * Space Complexity: O(n²) for the distance matrix
     */
    static TourResult
    calculateOptimalTour(const std::string &startCampus,
                         const std::vector<std::string> &campusesToVisit);

protected:
    /**
     * @brief Populates a distance matrix from database queries
     * 
     * Queries the distances database to fill a 2D matrix where
     * matrix[i][j] represents the distance between campus i and campus j.
     * The matrix is symmetric (distance A→B = distance B→A).
     * 
     * @param campusNames Vector of campus names to include
     * @param matrix Reference to 2D vector to populate (must be pre-sized)
     * @param campusIndex Map from campus name to matrix index
     * 
     * @pre matrix must be sized to [n][n] where n = campusNames.size()
     * @pre campusIndex must contain all campus names with valid indices
     * 
     * Time Complexity: O(n) where n = number of campuses
     * - Makes n database queries
     * - Each query returns at most n-1 distances
     * 
     * Space Complexity: O(1) additional (matrix passed by reference)
     */
    static void getAllDistances(const std::vector<std::string> campusNames,
                                std::vector<std::vector<double>> &matrix,
                                std::map<std::string, int> &campusIndex);

    /**
     * @brief Recursive greedy nearest-neighbor tour construction
     * 
     * Starting from the current campus, recursively visits the nearest
     * unvisited campus until all campuses have been visited. Uses a
     * min-heap (LinkedHeapTree) for efficient nearest neighbor selection.
     * 
     * Algorithm:
     * 1. Add current campus to route and mark as visited
     * 2. Build min-heap of distances to unvisited campuses
     * 3. If heap is empty (base case), return
     * 4. Extract minimum, recurse to that campus
     * 
     * @param current Index of the current campus in the distance matrix
     * @param distances 2D distance matrix (adjacency matrix)
     * @param visited Boolean vector tracking visited campuses
     * @param route Vector to populate with campus indices in visit order
     * 
     * @note Modifies visited and route in-place
     * 
     * Time Complexity: O(n²) where n = number of campuses
     * - Called n times (once per campus)
     * - Each call builds heap of up to n elements: O(n log n)
     * - Total: O(n² log n), simplified to O(n²) for Big-O analysis
     * 
     * Space Complexity: O(n) for recursion stack and heap
     */
    static void visit(const int current,
                      const std::vector<std::vector<double>> &distances,
                      std::vector<bool> &visited, 
                      std::vector<int> &route);

    /**
     * @brief 2-opt local optimization for tour improvement
     * 
     * Iteratively improves the tour by checking if reversing any
     * subsequence would reduce total distance. This is a local
     * optimization that can escape some suboptimal greedy choices.
     * 
     * Algorithm:
     * For each pair of non-adjacent edges (A-B) and (C-D):
     *   - Compare current distance: A→B + C→D
     *   - With proposed swap: A→C + B→D (reversing B...C segment)
     *   - If proposed is shorter, reverse the segment
     * Repeat until no improvement found.
     * 
     * @param route Vector of campus indices to optimize (modified in-place)
     * @param dist Distance matrix for distance lookups
     * 
     * @note This is a local optimization, not guaranteed globally optimal
     * @warning Modifies route in-place
     * 
     * Time Complexity: O(n²) per iteration
     * - Nested loops check all pairs: O(n²)
     * - Typically converges in few iterations for small n
     * - Worst case: O(n³) but rare in practice
     * 
     * Space Complexity: O(1) - operates in-place
     */
    static void twoOpt(std::vector<int> &route,
                       const std::vector<std::vector<double>> &dist);
};

#endif
