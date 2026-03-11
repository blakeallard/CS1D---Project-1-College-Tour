# Big-O Analysis

This document provides time and space complexity analysis for key algorithms in the College Tour application.

---

## 1. TourPlanner::visit() - Greedy Nearest-Neighbor Algorithm

**Location**: `src/Database/src/TourPlanner.cpp`

**Purpose**: Recursively builds a tour by always visiting the nearest unvisited campus.

### Algorithm
```cpp
void TourPlanner::visit(const int current,
                        const vector<vector<double>>& distances,
                        vector<bool>& visited,
                        vector<int>& route)
{
    route.push_back(current);           // O(1)
    visited[current] = true;            // O(1)

    LinkedHeapTree<double, int> heap;

    // Insert all unvisited campuses into heap
    for (int i = 0; i < visited.size(); i++) {    // O(n) iterations
        if (!visited[i])
            heap.insert(distances[current][i], i); // O(log n) per insert
    }

    if (heap.empty())
        return;                          // Base case

    int next = heap.removeMin();         // O(log n)
    visit(next, distances, visited, route); // Recursive call
}
```

### Time Complexity Analysis

| Component | Complexity | Explanation |
|-----------|------------|-------------|
| Base operations | O(1) | push_back, array access |
| Heap construction | O(n log n) | n insertions, each O(log n) |
| Extract minimum | O(log n) | Heap removeMin operation |
| Recursive calls | n times | Called once per campus |

**Total Time Complexity: O(n^2 log n)**

- The function is called n times (once per campus)
- Each call builds a heap with up to n elements: O(n log n)
- Total: n * O(n log n) = O(n^2 log n)

For practical purposes with small n (11-15 campuses), this simplifies to **O(n^2)**.

### Space Complexity: O(n)
- Recursion stack depth: O(n)
- Heap size: O(n) per call (but only one active at a time)

---

## 2. TourPlanner::twoOpt() - Local Optimization

**Location**: `src/Database/src/TourPlanner.cpp`

**Purpose**: Improves tour quality by reversing segments that reduce total distance.

### Algorithm
```cpp
void TourPlanner::twoOpt(vector<int>& route,
                         const vector<vector<double>>& dist)
{
    bool improved = true;

    while (improved) {                              // Multiple iterations
        improved = false;

        for (int i = 1; i < route.size() - 2; i++) {      // O(n)
            for (int j = i + 1; j < route.size() - 1; j++) { // O(n)
                int A = route[i - 1];
                int B = route[i];
                int C = route[j];
                int D = route[j + 1];

                double current = dist[A][B] + dist[C][D];   // O(1)
                double proposed = dist[A][C] + dist[B][D];  // O(1)

                if (proposed < current) {
                    reverse(route.begin() + i,
                           route.begin() + j + 1);          // O(n)
                    improved = true;
                }
            }
        }
    }
}
```

### Time Complexity Analysis

| Component | Complexity | Explanation |
|-----------|------------|-------------|
| Outer loop (while) | O(k) | k = number of improvement iterations |
| First for loop | O(n) | Iterates through route |
| Second for loop | O(n) | Nested iteration |
| Distance lookups | O(1) | Matrix access |
| Reverse operation | O(n) | std::reverse on segment |

**Single Iteration: O(n^2)**
- Nested loops: O(n^2)
- Each comparison: O(1)
- Reverse (when triggered): O(n)

**Total Time Complexity: O(k * n^2)**

Where k is the number of improvement iterations. In practice:
- k is typically small (often < 10) for small graphs
- Worst case: O(n^3) if improvements happen frequently

For this project with 11-15 campuses: **O(n^2) per iteration**

### Space Complexity: O(1)
- In-place modification of route vector
- Only constant additional variables

---

## 3. TourPlanner::calculateOptimalTour() - Main Tour Calculation

**Location**: `src/Database/src/TourPlanner.cpp`

**Purpose**: Orchestrates the complete tour planning process.

### Algorithm
```cpp
TourResult TourPlanner::calculateOptimalTour(
    const string& startCampus,
    const vector<string>& campusesToVisit)
{
    // Build campus list
    vector<string> campuses;
    campuses.push_back(startCampus);           // O(1)
    for (int i = 0; i < campusesToVisit.size(); i++) {
        campuses.push_back(campusesToVisit[i]); // O(n) total
    }
    int N = campuses.size();

    // Create index mapping
    map<string, int> campusIndex;
    for (int i = 0; i < N; i++) {
        campusIndex[campuses[i]] = i;          // O(n log n) total
    }

    // Initialize data structures
    vector<int> route;
    vector<bool> visited(N, 0);                 // O(n)
    vector<vector<double>> matrix(N, vector<double>(N, 0)); // O(n^2)

    // Populate distance matrix
    getAllDistances(campuses, matrix, campusIndex); // O(n) DB queries

    // Build route with greedy algorithm
    visit(0, matrix, visited, route);           // O(n^2)

    // Optimize with 2-opt
    twoOpt(route, matrix);                      // O(n^2) per iteration

    // Build result (omitted for brevity)       // O(n)
    return results;
}
```

### Time Complexity Analysis

| Step | Complexity | Explanation |
|------|------------|-------------|
| Build campus list | O(n) | Linear insertion |
| Create index map | O(n log n) | n map insertions |
| Initialize matrix | O(n^2) | 2D vector allocation |
| getAllDistances | O(n) | n database queries |
| visit (greedy) | O(n^2) | Nearest-neighbor algorithm |
| twoOpt | O(n^2) | Per iteration optimization |
| Build result | O(n) | Construct TourResult |

**Total Time Complexity: O(n^2)**

The dominant operations are:
1. Matrix initialization: O(n^2)
2. Greedy algorithm: O(n^2)
3. 2-opt optimization: O(n^2) per iteration

### Space Complexity: O(n^2)
- Distance matrix: O(n^2)
- Campus list: O(n)
- Route vector: O(n)
- Visited array: O(n)
- Index map: O(n)

---

## 4. LinkedHeapTree::insert() - Heap Insertion

**Location**: `src/Database/headers/LinkedHeapTree.h`

**Purpose**: Insert a new element while maintaining heap property.

### Algorithm
```cpp
Position insert(Key k, Value v) {
    Entry* newEntry = new Entry(k, v);    // O(1)
    Node* newNode = new Node;              // O(1)
    
    newNode->entry = newEntry;
    newEntry->node = newNode;
    nodeCount++;

    if (!root) {
        root = newNode;
        return Position(newEntry);
    }

    int lastIndex = size();
    int parentIndex = lastIndex >> 1;      // O(1)
    
    Node* parent = findNode(parentIndex);  // O(log n)
    newNode->parent = parent;
    
    if ((lastIndex & 1) == 0)
        parent->left = newNode;
    else
        parent->right = newNode;

    upHeapBubble(Position(newEntry));      // O(log n)
    return Position(newEntry);
}
```

### Time Complexity Analysis

| Operation | Complexity | Explanation |
|-----------|------------|-------------|
| Memory allocation | O(1) | Constant time |
| findNode | O(log n) | Bit manipulation traversal |
| upHeapBubble | O(log n) | Bubble up tree height |

**Total Time Complexity: O(log n)**

### Space Complexity: O(1)
- Creates one new Entry and Node (constant size)

---

## Summary

| Method | Time Complexity | Space Complexity |
|--------|----------------|------------------|
| `visit()` | O(n^2) | O(n) |
| `twoOpt()` | O(n^2) per iteration | O(1) |
| `calculateOptimalTour()` | O(n^2) | O(n^2) |
| `LinkedHeapTree::insert()` | O(log n) | O(1) |
| `LinkedHeapTree::removeMin()` | O(log n) | O(1) |

### Overall Algorithm Complexity

The complete tour calculation runs in **O(n^2)** time where n is the number of campuses. This is efficient for the project requirements (11-15 campuses) and provides near-optimal routes through the combination of:

1. **Greedy construction**: O(n^2) - Builds a good initial tour
2. **2-opt improvement**: O(n^2) - Refines the tour locally

Note: The globally optimal solution (true shortest path visiting all nodes) is NP-hard (Traveling Salesman Problem). Our greedy + 2-opt approach provides a practical approximation in polynomial time.
