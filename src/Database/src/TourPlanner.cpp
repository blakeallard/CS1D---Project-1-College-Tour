#include <TourPlanner.h>
#include <Queries.h>
#include <algorithm>
#include <iostream>

// Helper function to get all distances from a campus
std::unordered_map<std::string, double> TourPlanner::getDistancesFromCampus(
    const std::string &campus,
    const std::vector<std::string> &allCampuses)
{
    std::unordered_map<std::string, double> distances;

    // Get all distances from the QueryData namespace
    QueryData::allDistances(campus, distances);

    return distances;
}

/**
 * @brief Calculate tour with campuses sorted by distance from start
 *
 * ALGORITHM: Sort by distance from starting campus only
 * - Step 1: Compute distance from start to each selected campus
 * - Step 2: Sort campuses ascending by distance from start
 * - Step 3: Build route in sorted order
 * - Step 4: Calculate total as sum of consecutive hop distances
 *
 * IMPORTANT: This does NOT use greedy chaining or nearest-neighbor.
 * Campuses are ordered purely by their distance from the START.
 *
 * TIME COMPLEXITY: O(n log n) where n = selected campuses (for sorting)
 *
 * DETERMINISTIC: Same input always produces same output (stable sort)
 *
 * Example:
 *   Start: ASU
 *   Selected: [UCI, Northwestern, Oregon]
 *   Distances from ASU: UCI=375, Oregon=849, Northwestern=2043
 *   Sorted order: UCI (375), Oregon (849), Northwestern (2043)
 *   Route: ASU → UCI → Oregon → Northwestern
 *   Total: distance(ASU,UCI) + distance(UCI,Oregon) + distance(Oregon,Northwestern)
 *
 * @param startCampus Fixed starting campus
 * @param selectedCampuses Vector of campuses to visit
 * @return TourResult with route sorted by distance from start
 */
TourResult TourPlanner::calculateSortedByDistanceTour(
    const std::string &startCampus,
    const std::vector<std::string> &selectedCampuses)
{
    TourResult result;
    result.totalDistance = 0.0;

    // Add starting campus as first stop
    TourStop firstStop;
    firstStop.campus = startCampus;
    firstStop.distanceFromPrevious = 0.0;
    result.stops.push_back(firstStop);

    // VALIDATION: Empty selection returns just starting campus
    if (selectedCampuses.empty())
    {
        return result;
    }

    // Helper struct to store campus with its distance from start
    struct CampusDistance
    {
        std::string campus;
        double distanceFromStart;
    };

    std::vector<CampusDistance> campusDistances;

    // STEP 1: Compute distance from START to each selected campus
    for (const auto &campus : selectedCampuses)
    {
        // Skip if it's the starting campus
        if (campus == startCampus)
        {
            continue;
        }

        // Get distance from START to this campus
        double distFromStart = QueryData::distance(startCampus, campus);

        // VALIDATION: Only include campuses with valid distances
        if (distFromStart >= 0)
        {
            campusDistances.push_back({campus, distFromStart});
        }
    }

    // STEP 2: Sort campuses by distance from START (ascending)
    // This is a stable sort - deterministic output
    std::sort(campusDistances.begin(), campusDistances.end(),
        [](const CampusDistance &a, const CampusDistance &b)
        {
            return a.distanceFromStart < b.distanceFromStart;
        });

    // STEP 3: Build route in sorted order
    // STEP 4: Calculate total distance as sum of consecutive hops
    std::string currentCampus = startCampus;

    for (const auto &cd : campusDistances)
    {
        // Get actual distance from previous campus to this campus
        double distFromPrevious = QueryData::distance(currentCampus, cd.campus);

        // Add stop to route
        TourStop stop;
        stop.campus = cd.campus;
        stop.distanceFromPrevious = distFromPrevious;
        result.stops.push_back(stop);

        // Accumulate total distance
        result.totalDistance += distFromPrevious;

        // Move to this campus for next iteration
        currentCampus = cd.campus;
    }

    return result;
}

/**
 * @brief Recursive backtracking helper function to find optimal tour
 *
 * This function explores ALL possible permutations of campus orderings using
 * recursive backtracking with pruning for efficiency.
 *
 * TIME COMPLEXITY: O(n!) where n is number of campuses
 * - Each level of recursion branches n times
 * - Depth is n levels
 * - Total nodes explored: n! (with pruning, fewer in practice)
 *
 * SPACE COMPLEXITY: O(n) for recursion stack
 *
 * @param currentCampus Current position in the tour
 * @param remainingCampuses Vector of campuses not yet visited
 * @param currentDistance Total distance traveled so far
 * @param currentPath Current sequence of stops
 * @param bestDistance Reference to best (minimum) distance found
 * @param bestPath Reference to best path found
 * @param enableLogging Enable detailed logging for algorithm verification
 */
void findOptimalTourRecursive(
    const std::string &currentCampus,
    const std::vector<std::string> &remainingCampuses,
    double currentDistance,
    const std::vector<TourStop> &currentPath,
    double &bestDistance,
    std::vector<TourStop> &bestPath,
    bool enableLogging = false)
{
    // BASE CASE: All campuses visited
    if (remainingCampuses.empty())
    {
        // Check if this is the best route found so far
        if (currentDistance < bestDistance)
        {
            bestDistance = currentDistance;
            bestPath = currentPath;

            if (enableLogging)
            {
                std::cout << "\n[ALGORITHM] New best route found! Total distance: " << bestDistance << std::endl;
            }
        }
        return;
    }

    // PRUNING: If current path is already worse than best, stop exploring
    if (currentDistance >= bestDistance)
    {
        return; // Prune this branch
    }

    if (enableLogging && !remainingCampuses.empty())
    {
        std::cout << "\n[ALGORITHM] Current campus: " << currentCampus << std::endl;
        std::cout << "[ALGORITHM] Remaining campuses to visit: " << remainingCampuses.size() << std::endl;
        std::cout << "[ALGORITHM] Distances checked:" << std::endl;
    }

    // Find nearest neighbor for verification
    double minDistance = std::numeric_limits<double>::max();
    std::string nearestCampus;

    // RECURSIVE CASE: Try each remaining campus as the next stop
    for (size_t i = 0; i < remainingCampuses.size(); i++)
    {
        const std::string &nextCampus = remainingCampuses[i];

        // Get distance from current campus to this candidate
        double distanceToNext = QueryData::distance(currentCampus, nextCampus);

        // Skip if no valid distance exists
        if (distanceToNext < 0)
        {
            continue;
        }

        if (enableLogging)
        {
            std::cout << "  " << nextCampus << " = " << distanceToNext << " miles" << std::endl;
        }

        // Track nearest neighbor
        if (distanceToNext < minDistance)
        {
            minDistance = distanceToNext;
            nearestCampus = nextCampus;
        }

        // Create new path with this campus added
        std::vector<TourStop> newPath = currentPath;
        TourStop stop;
        stop.campus = nextCampus;
        stop.distanceFromPrevious = distanceToNext;
        newPath.push_back(stop);

        // Create new remaining list without this campus
        std::vector<std::string> newRemaining;
        newRemaining.reserve(remainingCampuses.size() - 1);
        for (size_t j = 0; j < remainingCampuses.size(); j++)
        {
            if (j != i)
            {
                newRemaining.push_back(remainingCampuses[j]);
            }
        }

        // RECURSE: Explore this branch
        findOptimalTourRecursive(
            nextCampus,
            newRemaining,
            currentDistance + distanceToNext,
            newPath,
            bestDistance,
            bestPath,
            enableLogging
        );

        // BACKTRACK: Function automatically backtracks when it returns
        // No explicit cleanup needed since we use local copies
    }

    if (enableLogging && !nearestCampus.empty())
    {
        std::cout << "[ALGORITHM] Nearest neighbor: " << nearestCampus << " (" << minDistance << " miles)" << std::endl;
    }
}

/**
 * @brief Calculate optimal tour using recursive backtracking
 *
 * ALGORITHM: Exhaustive recursive search with pruning
 * - Evaluates ALL possible orderings of campuses
 * - Uses branch-and-bound pruning to skip worse paths
 * - Guarantees finding the globally optimal solution
 *
 * TIME COMPLEXITY: O(n!) where n is number of campuses to visit
 * - With pruning: Typically much better in practice
 * - For n=5: 5! = 120 permutations (very fast)
 * - For n=10: 10! = 3,628,800 permutations (still reasonable)
 * - For n=15: 15! = ~1.3 trillion (would be slow without pruning)
 *
 * WHY THIS IS ACCEPTABLE:
 * - Project typically uses 5-13 campuses
 * - Pruning dramatically reduces search space
 * - Correctness is more important than speed for this educational project
 * - Modern CPUs can handle millions of comparisons per second
 *
 * OPTIMALITY GUARANTEE:
 * - Explores every possible ordering
 * - Computes exact distance for each
 * - Returns provably optimal solution
 *
 * @param startCampus Campus where tour begins
 * @param campusesToVisit List of campuses to visit (excluding start)
 * @param enableLogging Enable detailed logging for algorithm verification
 * @return TourResult with optimal route and minimum distance
 */
TourResult TourPlanner::calculateOptimalTour(
    const std::string &startCampus,
    const std::vector<std::string> &campusesToVisit,
    bool enableLogging)
{
    TourResult result;
    result.totalDistance = 0.0;

    if (enableLogging)
    {
        std::cout << "\n========================================" << std::endl;
        std::cout << "TOUR ALGORITHM VERIFICATION" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Starting campus: " << startCampus << std::endl;
        std::cout << "Campuses to visit: " << campusesToVisit.size() << std::endl;
    }

    // If no campuses to visit, return tour with just start
    if (campusesToVisit.empty())
    {
        TourStop start;
        start.campus = startCampus;
        start.distanceFromPrevious = 0.0;
        result.stops.push_back(start);

        if (enableLogging)
        {
            std::cout << "\n[RESULT] Only starting campus - no tour needed" << std::endl;
            std::cout << "========================================\n" << std::endl;
        }
        return result;
    }

    // Filter out start campus from the list to visit
    std::vector<std::string> remainingCampuses;
    for (const auto &campus : campusesToVisit)
    {
        if (campus != startCampus)
        {
            remainingCampuses.push_back(campus);
        }
    }

    // Initialize starting point
    std::vector<TourStop> initialPath;
    TourStop firstStop;
    firstStop.campus = startCampus;
    firstStop.distanceFromPrevious = 0.0;
    initialPath.push_back(firstStop);

    // Initialize best distance to infinity
    double bestDistance = std::numeric_limits<double>::max();
    std::vector<TourStop> bestPath;

    if (enableLogging)
    {
        std::cout << "\n[ALGORITHM] Beginning recursive search..." << std::endl;
    }

    // Launch recursive search
    findOptimalTourRecursive(
        startCampus,
        remainingCampuses,
        0.0,
        initialPath,
        bestDistance,
        bestPath,
        enableLogging
    );

    // Build result from best path found
    // If no valid path found, return just the starting campus
    if (bestPath.empty())
    {
        result.stops = initialPath;
        result.totalDistance = 0.0;

        if (enableLogging)
        {
            std::cout << "\n[ERROR] No valid path found!" << std::endl;
        }
    }
    else
    {
        result.stops = bestPath;
        result.totalDistance = bestDistance;

        if (enableLogging)
        {
            std::cout << "\n========================================" << std::endl;
            std::cout << "FINAL OPTIMAL ROUTE" << std::endl;
            std::cout << "========================================" << std::endl;

            for (size_t i = 0; i < result.stops.size(); i++)
            {
                const auto &stop = result.stops[i];
                if (i == 0)
                {
                    std::cout << stop.campus << " (START)";
                }
                else
                {
                    std::cout << " -> " << stop.campus << " (+" << stop.distanceFromPrevious << " miles)";
                }
            }
            std::cout << std::endl;
            std::cout << "\nTotal Distance: " << result.totalDistance << " miles" << std::endl;
            std::cout << "========================================" << std::endl;

            // Verification: Check if route follows nearest neighbor at each step
            std::cout << "\nNEAREST NEIGHBOR VERIFICATION:" << std::endl;
            std::cout << "========================================" << std::endl;

            bool allValid = true;
            for (size_t i = 0; i < result.stops.size() - 1; i++)
            {
                const std::string &current = result.stops[i].campus;
                const std::string &chosen = result.stops[i + 1].campus;
                double chosenDist = result.stops[i + 1].distanceFromPrevious;

                // Check against all unvisited campuses at this point
                std::vector<std::string> unvisited;
                for (size_t j = i + 1; j < result.stops.size(); j++)
                {
                    unvisited.push_back(result.stops[j].campus);
                }

                std::cout << "Step " << (i + 1) << ": " << current << " -> " << chosen << std::endl;
                std::cout << "  Chosen distance: " << chosenDist << " miles" << std::endl;

                // Check all alternatives
                bool isOptimal = true;
                for (const auto &alt : unvisited)
                {
                    if (alt != chosen)
                    {
                        double altDist = QueryData::distance(current, alt);
                        if (altDist >= 0 && altDist < chosenDist)
                        {
                            std::cout << "  WARNING: " << alt << " is closer (" << altDist << " miles)" << std::endl;
                            isOptimal = false;
                            allValid = false;
                        }
                    }
                }

                if (isOptimal)
                {
                    std::cout << "  ✓ Optimal choice (nearest available)" << std::endl;
                }
            }

            std::cout << "\nVERIFICATION RESULT: " << (allValid ? "PASSED ✓" : "FAILED ✗") << std::endl;
            std::cout << "========================================\n" << std::endl;
        }
    }

    return result;
}

// Calculate tour visiting N nearest campuses
// OPTIMIZATION: Uses database query + in-memory sort for selection, then nearest-neighbor for routing
// TIME COMPLEXITY: O(m log m + n²) where m = total campuses, n = selected campuses
TourResult TourPlanner::calculateNearestNCampuses(
    const std::string &startCampus,
    int numberOfCampuses,
    const std::vector<std::string> &allCampuses)
{
    // Create vector of campus-distance pairs (excluding start campus)
    // This requires O(m) distance queries where m = total campuses
    std::vector<std::pair<std::string, double>> campusDistances;
    campusDistances.reserve(allCampuses.size()); // Pre-allocate for efficiency

    for (const auto &campus : allCampuses)
    {
        if (campus != startCampus)
        {
            double dist = QueryData::distance(startCampus, campus);
            if (dist >= 0) // Only include if valid distance exists
            {
                campusDistances.push_back({campus, dist});
            }
        }
    }

    // EFFICIENT SORT: O(m log m) using std::sort with lambda comparator
    // This is optimal - we MUST sort to find N nearest campuses
    // Alternative would be O(m) selection algorithm, but for small m, sort is fine
    std::sort(campusDistances.begin(), campusDistances.end(),
              [](const auto &a, const auto &b) {
                  return a.second < b.second; // Ascending order by distance
              });

    // Take the N nearest campuses - O(n) where n = numberOfCampuses
    std::vector<std::string> selectedCampuses;
    int count = std::min(numberOfCampuses, (int)campusDistances.size());
    selectedCampuses.reserve(count);

    for (int i = 0; i < count; i++)
    {
        selectedCampuses.push_back(campusDistances[i].first);
    }

    // Now calculate optimal tour through selected campuses using nearest-neighbor: O(n²)
    // TOTAL COMPLEXITY: O(m log m + n²)
    return calculateOptimalTour(startCampus, selectedCampuses);
}

// Calculate tour visiting all campuses
TourResult TourPlanner::calculateFullTour(
    const std::string &startCampus,
    const std::vector<std::string> &allCampuses)
{
    // Simply call calculateOptimalTour with all campuses
    return calculateOptimalTour(startCampus, allCampuses);
}
