#include <LinkedHeapTree.h>
#include <Queries.h>
#include <TourPlanner.h>
#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

void TourPlanner::getAllDistances(const vector<string> campusNames,
                                  vector<vector<double>> &matrix,
                                  map<string, int> &campusIndex)
{

    for (int i = 0; i < campusNames.size(); i++)
    {
        // Query every college one by one
        QueryData::QueryResult allDistances = QueryData::selectRows(
            "distances.db", "distances",
            {"starting_college", "ending_college", "distance"},
            {"starting_college"}, {campusNames[i]});

        // every query returns the rows in database, we go through every row
        for (auto &row : allDistances)
        {
            string start_college = get<string>(row.at("starting_college"));
            string end_college   = get<string>(row.at("ending_college"));
            double distance      = get<double>(row.at("distance"));

            // if the campuses are in this row we add the distance to the matrix
            if (campusIndex.find(start_college) != campusIndex.end() &&
                campusIndex.find(end_college) != campusIndex.end())
            {
                int i = campusIndex[start_college];
                int j = campusIndex[end_college];

                matrix[i][j] = distance;
                matrix[j][i] = distance;
            }
        }
    }
}

void TourPlanner::visit(const int current,
                        const vector<vector<double>> &distances,
                        vector<bool> &visited, vector<int> &route)
{
    route.push_back(current);
    visited[current] = true;

    LinkedHeapTree<double, int> heap;

    // If we have visited something, it is not part of the new heap
    for (int i = 0; i < visited.size(); i++)
    {
        if (!visited[i])
            heap.insert(distances[current][i], i);
    }

    // base case
    if (heap.empty())
        return;

    // The next one we go to is the top of the min heap
    int next = heap.removeMin();

    visit(next, distances, visited, route);
}

void TourPlanner::twoOpt(vector<int> &route, const vector<vector<double>> &dist)
{
    bool improved = true;

    while (improved)
    {
        improved = false;

        for (int i = 1; i < route.size() - 2; i++)
        {
            for (int j = i + 1; j < route.size() - 1; j++)
            {
                int A = route[i - 1];
                int B = route[i];
                int C = route[j];
                int D = route[j + 1];

                double current = dist[A][B] + dist[C][D];

                double proposed = dist[A][C] + dist[B][D];

                if (proposed < current)
                {
                    reverse(route.begin() + i, route.begin() + j + 1);

                    improved = true;
                }
            }
        }
    }
}

TourResult TourPlanner::calculateOptimalTour(
    const std::string &startCampus,
    const std::vector<std::string> &campusesToVisit)
{

    // Getting all the campuses to visit
    vector<string> campuses;
    campuses.push_back(startCampus);
    for (int i = 0; i < campusesToVisit.size(); i++)
    {
        campuses.push_back(campusesToVisit[i]);
    }
    int N = campuses.size();

    // Setting up for calculations
    // Associates all strings with index values
    map<string, int> campusIndex;
    int nextIndex = 0;
    for (int i = 0; i < N; i++)
    {
        campusIndex[campuses[i]] = nextIndex;
        nextIndex++;
    }

    // sets up needed vectors
    vector<int> route;
    vector<bool> visited(N, 0);
    vector<vector<double>> allDistancesMatrix(N, std::vector<double>(N, 0));

    getAllDistances(campuses, allDistancesMatrix, campusIndex);
    // beggining of the recursive loop, populates route using a greedy algorithm
    visit(0, allDistancesMatrix, visited, route);
    // somewhat optimizes greedy algo with local optimizations
    twoOpt(route, allDistancesMatrix);
    // Technically can still not be the truest best path but that's an np hard
    // problem and we just in a intro data structs class

    // Setting up route to the spec
    double total = 0;
    TourResult results;
    TourStop stop1;
    stop1.campus               = startCampus;
    stop1.distanceFromPrevious = 0;
    results.stops.push_back(stop1);

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
