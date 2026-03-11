# Data Structures Documentation

This document identifies and describes all data structures used in the College Tour application.

## 1. LinkedHeapTree (Min-Heap Priority Queue)

**Location**: `src/Database/headers/LinkedHeapTree.h`

**Type**: Template-based Binary Min-Heap using linked tree structure

**Purpose**: Efficiently selects the nearest unvisited campus during tour path calculation.

### Structure
```cpp
template <typename Key, typename Value>
class LinkedHeapTree {
    struct Entry {
        Key key;           // Distance (priority)
        Value element;     // Campus index
        Node* node;        // Pointer to containing node
    };
    
    struct Node {
        Entry* entry;
        Node* parent;
        Node* left;
        Node* right;
    };
    
    Node* root;
    int nodeCount;
};
```

### Key Operations
| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| `insert(key, value)` | O(log n) | Add element with priority |
| `removeMin()` | O(log n) | Extract minimum element |
| `min()` | O(1) | Peek at minimum element |
| `empty()` | O(1) | Check if heap is empty |
| `size()` | O(1) | Get element count |

### Usage in Project
Used by `TourPlanner::visit()` to implement greedy nearest-neighbor selection:
```cpp
LinkedHeapTree<double, int> heap;
for (int i = 0; i < visited.size(); i++) {
    if (!visited[i])
        heap.insert(distances[current][i], i);
}
int nearest = heap.removeMin();
```

---

## 2. 2D Vector (Adjacency Matrix)

**Location**: `src/Database/src/TourPlanner.cpp`

**Type**: `std::vector<std::vector<double>>`

**Purpose**: Stores distances between all pairs of campuses for efficient O(1) lookup.

### Structure
```cpp
vector<vector<double>> allDistancesMatrix(N, vector<double>(N, 0));
// matrix[i][j] = distance from campus i to campus j
```

### Properties
- **Size**: N x N where N = number of campuses in tour
- **Symmetric**: `matrix[i][j] == matrix[j][i]`
- **Diagonal**: `matrix[i][i] == 0` (distance to self)

### Key Operations
| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| Access `matrix[i][j]` | O(1) | Get distance between campuses |
| Initialize | O(n^2) | Create N x N matrix |

### Usage in Project
```cpp
// Populate from database
getAllDistances(campuses, allDistancesMatrix, campusIndex);

// Use for distance lookups in algorithms
double dist = allDistancesMatrix[route[i]][route[i-1]];
```

---

## 3. std::map (Campus Index Mapping)

**Location**: `src/Database/src/TourPlanner.cpp`

**Type**: `std::map<std::string, int>`

**Purpose**: Maps campus names to matrix indices for O(log n) lookup.

### Structure
```cpp
map<string, int> campusIndex;
// "Saddleback College" -> 0
// "UCLA" -> 1
// "Arizona State University" -> 2
```

### Key Operations
| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| `insert` | O(log n) | Add campus-index pair |
| `find(key)` | O(log n) | Look up index by name |
| `at(key)` | O(log n) | Access index by name |

### Usage in Project
```cpp
// Build index mapping
for (int i = 0; i < N; i++) {
    campusIndex[campuses[i]] = i;
}

// Use for matrix access
int i = campusIndex[start_college];
int j = campusIndex[end_college];
matrix[i][j] = distance;
```

---

## 4. std::vector (Dynamic Arrays)

**Location**: Throughout the codebase

**Type**: `std::vector<T>`

**Purpose**: Dynamic storage for routes, campus lists, and query results.

### Variants Used

#### Route Vector
```cpp
vector<int> route;  // Ordered list of campus indices
```

#### Visited Tracking
```cpp
vector<bool> visited(N, false);  // Track visited campuses
```

#### Campus List
```cpp
vector<string> campuses;  // List of campus names
```

### Key Operations
| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| `push_back` | O(1) amortized | Add element to end |
| `operator[]` | O(1) | Access by index |
| `size()` | O(1) | Get element count |
| `begin()/end()` | O(1) | Iterator access |

---

## 5. std::unordered_map (Database Row)

**Location**: `src/Database/headers/Queries.h`

**Type**: `std::unordered_map<std::string, SQLiteValue>`

**Purpose**: Represents a single database row with column-value pairs.

### Structure
```cpp
using SQLiteValue = std::variant<int, double, std::string, std::nullptr_t>;
using Row = std::unordered_map<std::string, SQLiteValue>;
using QueryResult = std::vector<Row>;
```

### Key Operations
| Operation | Time Complexity | Description |
|-----------|----------------|-------------|
| `at(key)` | O(1) average | Access value by column name |
| `find(key)` | O(1) average | Check if column exists |

### Usage in Project
```cpp
for (const auto& row : results) {
    string name = std::get<std::string>(row.at("college"));
    double dist = std::get<double>(row.at("distance"));
}
```

---

## 6. std::variant (Type-Safe Union)

**Location**: `src/Database/headers/Queries.h`

**Type**: `std::variant<int, double, std::string, std::nullptr_t>`

**Purpose**: Type-safe storage for SQLite values of different types.

### Structure
```cpp
using SQLiteValue = std::variant<int, double, std::string, std::nullptr_t>;
```

### Usage
```cpp
// Store different types
SQLiteValue intVal = 42;
SQLiteValue strVal = std::string("UCLA");
SQLiteValue dblVal = 25.5;

// Extract with type safety
double price = std::get<double>(row.at("price"));
string name = std::get<std::string>(row.at("name"));
```

---

## Summary Table

| Data Structure | Type | Location | Primary Purpose |
|---------------|------|----------|-----------------|
| LinkedHeapTree | Min-Heap (linked) | TourPlanner | Nearest-neighbor selection |
| 2D Vector | Adjacency Matrix | TourPlanner | Distance lookups |
| std::map | Red-Black Tree | TourPlanner | Campus name to index |
| std::vector | Dynamic Array | Throughout | Lists and sequences |
| std::unordered_map | Hash Table | Queries | Database row storage |
| std::variant | Type-Safe Union | Queries | SQLite value storage |
