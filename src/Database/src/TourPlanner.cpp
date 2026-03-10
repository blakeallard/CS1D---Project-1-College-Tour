#include <LinkedHeapTree.h>
#include <Queries.h>
#include <TourPlanner.h>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
using namespace std;

// Helper function to get all distances from a campus
void getAllDistances(const vector<string> campusNames,
                     vector<vector<double>> &matrix,
                     map<string, int> &campusIndex)
{
    int N = campusNames.size();

    // Fill the 2d array
    for (int i = 0; i < N; i++)
    {
        QueryData::QueryResult allDistances = QueryData::selectRows(
            "distances.db", "distances",
            {"starting_college", "ending_college", "distance"},
            {"starting_college"}, {campusNames[i]});
        for (auto &row : allDistances)
        {
            string start_college = get<string>(row.at("starting_college"));
            string end_college   = get<string>(row.at("ending_college"));
            double distance      = get<double>(row.at("distance"));

            if (campusIndex.find(start_college) != campusIndex.end() &&
                campusIndex.find(end_college) != campusIndex.end())
            {
                int i = campusIndex[start_college];
                int j = campusIndex[end_college];

                // cout << "Added college: " << start_college << "," <<
                // end_college
                //      << "," << distance << endl;
                matrix[i][j] = distance;
                matrix[j][i] = distance;
            }
        }
    }

    /*
    cout << "MATRIX TEST:\n";
    cout << setw(50) << "";
    cout << "|";
    for (int i = 0; i < campusNames.size(); i++)
    {
        cout << campusNames[i] << "|";
    }

    cout << endl;

    for (int row = 0; row < campusNames.size(); row++)
    {
        cout << setw(50) << left << campusNames[row] << "|";
        for (int col = 0; col < N; col++)
        {
            cout << setw(campusNames[col].size()) << left << matrix[row][col];
            cout << "|";
        }
        cout << endl;
    }
    */
}

void visit(const int current, const vector<vector<double>> &distances,
           vector<bool> &visited, vector<int> &route)
{
    visited[current] = true;
    route.push_back(current);

    LinkedHeapTree<double, int> heap;

    for (int i = 0; i < visited.size(); i++)
    {
        if (!visited[i])
            heap.insert(distances[current][i], i);
    }

    if (heap.empty())
        return;

    heap.prettyPrint(heap.min());
    int next = heap.removeMin();

    // cout << "Travel to " << next << endl;

    visit(next, distances, visited, route);
}

TourResult TourPlanner::calculateOptimalTour(
    const std::string &startCampus,
    const std::vector<std::string> &campusesToVisit)
{

    vector<string> campuses;
    campuses.resize(1);
    campuses.push_back(startCampus);
    for (int i = 0; i < campusesToVisit.size(); i++)
    {
        campuses.push_back(campusesToVisit[i]);
    }
    int N = campuses.size();

    // Associates all strings with index values
    map<string, int> campusIndex;
    int nextIndex = 0;
    for (int i = 0; i < N; i++)
    {
        campusIndex[campuses[i]] = nextIndex;
        nextIndex++;
    }

    vector<int> route(N, 0);
    vector<bool> visited(N, 0);
    vector<vector<double>> allDistancesMatrix(N, std::vector<double>(N, 0));

    getAllDistances(campuses, allDistancesMatrix, campusIndex);
    visit(0, allDistancesMatrix, visited, route);

    cout << "ROUTE:\n";
    for (int i = 0; i < route.size(); i++)
    {
        for (int j = 0; j < campuses.size(); j++)
        {
            if (campusIndex[campuses[j]] == route[i])
                cout << campuses[j] << ",";
        }
    }
    cout << "\nROUTE END:\n";

    return TourResult();
}
