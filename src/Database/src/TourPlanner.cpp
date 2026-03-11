/**
 * @file TourPlanner.cpp
 * @brief Implementation of tour planning algorithms
 * 
 * Implements greedy nearest-neighbor tour construction with 2-opt
 * local optimization for finding efficient routes through campuses.
 */

#include <LinkedHeapTree.h>
#include <Queries.h>
#include <TourPlanner.h>
#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

/**
 * @brief Populates distance matrix from database
 * 
 * Queries the distances database for each campus and fills the
 * symmetric distance matrix. Uses campusIndex to map campus names
 * to matrix indices.
 * 
 * Time Complexity: O(n) database queries, O(n²) total distance assignments
 */
void TourPlanner::getAllDistances(const vector<string> campusNames,
                                  vector<vector<double>> &matrix,
                                  map<string, int> &campusIndex)
{
    // Query distances for each campus - O(n) queries
    for (int i = 0; i < campusNames.size(); i++)
    {
        // Database query for all distances from this campus
        QueryData::QueryResult allDistances = QueryData::selectRows(
            "distances.db", "distances",
            {"starting_college", "ending_college", "distance"},
            {"starting_college"}, {campusNames[i]});

        // Process each row from the query result
        for (auto &row : allDistances)
        {
            string start_college = get<string>(row.at("starting_college"));
            string end_college   = get<string>(row.at("ending_college"));
            double distance      = get<double>(row.at("distance"));

            // Only add if both campuses are in our tour set
            if (campusIndex.find(start_college) != campusIndex.end() &&
                campusIndex.find(end_college) != campusIndex.end())
            {
                int i = campusIndex[start_college];
                int j = campusIndex[end_college];

                // Fill both directions (symmetric matrix)
                matrix[i][j] = distance;
                matrix[j][i] = distance;
            }
        }
    }
}

/**
 * @brief Recursive greedy nearest-neighbor traversal
 * 
 * Uses a min-heap (LinkedHeapTree) to efficiently select the
 * nearest unvisited campus at each step. Recursively builds
 * the tour route.
 * 
 * Time Complexity: O(n²) overall
 * - n recursive calls
 * - Each call: O(n) heap insertions, O(log n) extraction
 */
void TourPlanner::visit(const int current,
                        const vector<vector<double>> &distances,
                        vector<bool> &visited, vector<int> &route)
{
    // Add current campus to route and mark visited
    route.push_back(current);
    visited[current] = true;

    // Build min-heap of distances to unvisited campuses
    // LinkedHeapTree provides O(log n) insert and O(log n) removeMin
    LinkedHeapTree<double, int> heap;

    // Insert all unvisited campuses with their distances - O(n log n)
    for (int i = 0; i < visited.size(); i++)
    {
        if (!visited[i])
            heap.insert(distances[current][i], i);
    }

    // Base case: all campuses visited
    if (heap.empty())
        return;

    // Greedy choice: select nearest unvisited campus - O(log n)
    int next = heap.removeMin();

    // Recursive call to visit next campus
    visit(next, distances, visited, route);
}

/**
 * @brief 2-opt local optimization
 * 
 * Iteratively checks all pairs of edges and reverses segments
 * when it would reduce total distance. Continues until no
 * further improvements can be made.
 * 
 * Time Complexity: O(n²) per iteration, typically few iterations
 */
void TourPlanner::twoOpt(vector<int> &route, const vector<vector<double>> &dist)
{
    bool improved = true;

    // Continue until no improvement found
    while (improved)
    {
        improved = false;

        // Check all pairs of non-adjacent edges - O(n²)
        for (int i = 1; i < route.size() - 2; i++)
        {
            for (int j = i + 1; j < route.size() - 1; j++)
            {
                // Current edges: A-B and C-D
                int A = route[i - 1];
                int B = route[i];
                int C = route[j];
                int D = route[j + 1];

                // Current total distance of these two edges
                double current = dist[A][B] + dist[C][D];

                // Proposed distance if we reverse segment B...C
                // New edges would be: A-C and B-D
                double proposed = dist[A][C] + dist[B][D];

                // If proposed is better, reverse the segment
                if (proposed < current)
                {
                    // Reverse segment from B to C (inclusive)
                    reverse(route.begin() + i, route.begin() + j + 1);

                    improved = true;
                }
            }
        }
    }
}

/**
 * @brief Main tour calculation entry point
 * 
 * Orchestrates the tour planning process:
 * 1. Build campus list and index mapping
 * 2. Construct distance matrix from database
 * 3. Run greedy nearest-neighbor algorithm
 * 4. Optimize with 2-opt
 * 5. Convert to TourResult format
 * 
 * Time Complexity: O(n²) where n = number of campuses
 * Space Complexity: O(n²) for distance matrix
 */
TourResult TourPlanner::calculateOptimalTour(
    const std::string &startCampus,
    const std::vector<std::string> &campusesToVisit)
{
    // Build complete campus list with start campus first
    vector<string> campuses;
    campuses.push_back(startCampus);
    for (int i = 0; i < campusesToVisit.size(); i++)
    {
        campuses.push_back(campusesToVisit[i]);
    }
    int N = campuses.size();

    // Create index mapping: campus name -> matrix index
    // Using map for O(log n) lookup
    map<string, int> campusIndex;
    int nextIndex = 0;
    for (int i = 0; i < N; i++)
    {
        campusIndex[campuses[i]] = nextIndex;
        nextIndex++;
    }

    // Initialize data structures
    vector<int> route;                                           // Final route (indices)
    vector<bool> visited(N, 0);                                  // Visited tracking
    vector<vector<double>> allDistancesMatrix(N, vector<double>(N, 0));  // Distance matrix

    // Populate distance matrix from database - O(n)
    getAllDistances(campuses, allDistancesMatrix, campusIndex);
    
    // Build initial route using greedy nearest-neighbor - O(n²)
    visit(0, allDistancesMatrix, visited, route);
    
    // Optimize route with 2-opt local search - O(n²) per iteration
    twoOpt(route, allDistancesMatrix);

    // Convert route indices to TourResult format
    double total = 0;
    TourResult results;
    
    // Add starting campus with 0 distance
    TourStop stop1;
    stop1.campus               = startCampus;
    stop1.distanceFromPrevious = 0;
    results.stops.push_back(stop1);

    // Add remaining stops with distances
    for (int i = 1; i < route.size(); i++)
    {
        TourStop stop;
        stop.campus               = campuses[route[i]];
        stop.distanceFromPrevious = allDistancesMatrix[route[i]][route[i - 1]];
        total += allDistancesMatrix[route[i]][route[i - 1]];
        results.stops.push_back(stop);
    }
    results.totalDistance = total;

    return results;
}
