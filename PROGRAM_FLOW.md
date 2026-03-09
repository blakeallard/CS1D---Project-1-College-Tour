# Program Flow

## Overview

This document describes how data flows through the College Tour application, from user interaction to database and back.

---

## General Flow: Frontend to Backend to Frontend

### 1. User Action (Frontend)
```
User interacts with UI (clicks button, submits form, etc.)
    |
    v
React component handles event (onClick, onSubmit)
    |
    v
Component calls axios.get() or axios.post() to API endpoint
```

### 2. API Request (Network)
```
HTTP request sent to backend
    |
    v
URL: http://localhost:18080/api/{Controller}/{action}
Method: GET, POST, PATCH, or DELETE
Body: JSON data (for POST/PATCH)
```

### 3. Route Handling (Backend - main.cpp)
```
Crow server receives request at port 18080
    |
    v
RouteRegister.h matches URL pattern to Controller
    |
    v
Appropriate Controller method called:
  - GET    -> Controller::read(id)
  - POST   -> Controller::create(req, id)
  - PATCH  -> Controller::patch(req, id)
  - DELETE -> Controller::remove(req, id)
```

### 4. Controller Processing (Backend - Controllers/)
```
Controller parses request
    |
    v
Controller calls business logic (TourPlanner, etc.)
    |
    v
Business logic queries database via QueryData namespace
    |
    v
Database returns results
    |
    v
Controller builds JSON response
```

### 5. Database Query (Backend - Database/)
```
QueryData::selectRows() or selectRowsWithQuery()
    |
    v
SQL query executed on SQLite database
    |
    v
Results returned as QueryResult (vector of rows)
```

### 6. Response (Backend to Frontend)
```
Controller returns crow::response with JSON
    |
    v
HTTP response sent back to frontend
    |
    v
axios receives response.data
    |
    v
React component updates state with data
    |
    v
UI re-renders to display results
```

---

## Specific Example: Saddleback Tour Calculation

This walks through exactly what happens when a user calculates a tour starting from Saddleback College.

### Step 1: Page Load - Fetching Available Campuses

**User Action:**
User navigates to Saddleback Tour page.

**Frontend (SaddlebackStart.jsx, lines 23-40):**
```javascript
useEffect(() => {
    const fetchCampuses = async () => {
        setLoading(true);
        const response = await axios.get("/api/University/from-Saddleback College");
        setCampuses(response.data.campuses || []);
        setLoading(false);
    };
    fetchCampuses();
}, []);
```

**Network Request:**
```
GET /api/University/from-Saddleback%20College
```

**Backend Route (main.cpp, line 14):**
```cpp
register_crud_routes<UniversityController>(app, "University/");
```
Routes to `UniversityController::read("from-Saddleback College")`

**Controller (UniversityController.cpp, lines 33-45):**
```cpp
// id = "from-Saddleback College"
// id.rfind("from-", 0) == 0 is true, so:
string startCampus = id.substr(5);  // "Saddleback College"

QueryData::QueryResult campuses = QueryData::selectRows(
    "distances.db", "distances", {"ending_college", "distance"},
    {"starting_college"}, {startCampus});
```

**Database Query:**
```sql
SELECT ending_college, distance 
FROM distances 
WHERE starting_college = 'Saddleback College';
```

**Database Returns:**
```
Arizona State University      | 379.0
MIT                           | 2989.0
Northwestern                  | 2033.0
Ohio State University         | 2252.0
University of Michigan        | 2249.0
UCI                           | 16.0
UCLA                          | 66.0
University of Oregon          | 906.0
University of the Pacific     | 390.0
University of Wisconsin       | 1981.0
```

**Controller Builds Response:**
```cpp
for (const auto &row : campuses) {
    result["campuses"][i]["name"] = get<string>(row.at("ending_college"));
    result["campuses"][i]["distance"] = get<double>(row.at("distance"));
    i++;
}
return crow::response(result);
```

**JSON Response:**
```json
{
  "campuses": [
    {"name": "Arizona State University", "distance": 379.0},
    {"name": "MIT", "distance": 2989.0},
    {"name": "Northwestern", "distance": 2033.0},
    ...
  ]
}
```

**Frontend Receives:**
```javascript
// response.data.campuses is now an array of 10 campus objects
setCampuses(response.data.campuses);
// UI renders checkboxes for each campus
```

---

### Step 2: User Selects Campuses and Submits

**User Action:**
- Enters name: "John Doe"
- Checks: Arizona State University, UCLA, UCI
- Clicks "Calculate Tour"

**Frontend State:**
```javascript
name = "John Doe"
selectedCampuses = {
    "Arizona State University": true,
    "University of California, Los Angeles (UCLA)": true,
    "University of California, Irvine (UCI)": true
}
```

**Frontend (SaddlebackStart.jsx, handleSubmit, lines 96-133):**
```javascript
const selected = Object.keys(selectedCampuses).filter(key => selectedCampuses[key]);
// selected = ["Arizona State University", "UCLA", "UCI"]

const response = await axios.post("/api/Tour/calculate", {
    startCampus: "Saddleback College",
    selectedCampuses: selected
});
```

**Network Request:**
```
POST /api/Tour/calculate
Content-Type: application/json

{
    "startCampus": "Saddleback College",
    "selectedCampuses": [
        "Arizona State University",
        "University of California, Los Angeles (UCLA)",
        "University of California, Irvine (UCI)"
    ]
}
```

---

### Step 3: Backend Calculates Optimal Tour

**Controller (TourController.cpp, lines 28-79):**
```cpp
// id = "calculate"
auto jsonData = crow::json::load(req.body);
string startCampus = jsonData["startCampus"].s();  // "Saddleback College"

vector<string> selectedCampuses;
for (const auto &campus : jsonData["selectedCampuses"]) {
    selectedCampuses.push_back(campus.s());
}
// selectedCampuses = ["Arizona State University", "UCLA", "UCI"]

TourResult tour = TourPlanner::calculateOptimalTour(startCampus, selectedCampuses);
```

**TourPlanner (TourPlanner.cpp, calculateOptimalTour, lines 286-444):**
```cpp
// Initialize
vector<TourStop> initialPath;
TourStop firstStop;
firstStop.campus = "Saddleback College";
firstStop.distanceFromPrevious = 0.0;
initialPath.push_back(firstStop);

// Launch recursive search through all permutations
findOptimalTourRecursive(
    "Saddleback College",
    remainingCampuses,  // ["ASU", "UCLA", "UCI"]
    0.0,
    initialPath,
    bestDistance,       // starts at infinity
    bestPath
);
```

**Recursive Algorithm Exploration:**
```
Permutation 1: Saddleback -> ASU -> UCLA -> UCI
  Distance: 379 + 426 + 51 = 856 miles
  
Permutation 2: Saddleback -> ASU -> UCI -> UCLA
  Distance: 379 + 375 + 51 = 805 miles
  
Permutation 3: Saddleback -> UCLA -> ASU -> UCI
  Distance: 66 + 426 + 375 = 867 miles
  
Permutation 4: Saddleback -> UCLA -> UCI -> ASU
  Distance: 66 + 51 + 375 = 492 miles
  
Permutation 5: Saddleback -> UCI -> ASU -> UCLA
  Distance: 16 + 375 + 426 = 817 miles
  
Permutation 6: Saddleback -> UCI -> UCLA -> ASU
  Distance: 16 + 51 + 426 = 493 miles

Best: Permutation 4 with 492 miles
```

**Database Queries During Calculation:**
```cpp
// For each distance lookup:
double dist = QueryData::distance("Saddleback College", "UCI");
```

```sql
SELECT distance FROM distances 
WHERE starting_college = 'Saddleback College' 
AND ending_college = 'University of California, Irvine (UCI)';
-- Returns: 16.0
```

**TourResult Built:**
```cpp
result.stops = [
    {campus: "Saddleback College", distanceFromPrevious: 0.0},
    {campus: "UCI", distanceFromPrevious: 16.0},
    {campus: "UCLA", distanceFromPrevious: 51.0},
    {campus: "Arizona State University", distanceFromPrevious: 426.0}
];
result.totalDistance = 493.0;
```

---

### Step 4: Response Sent to Frontend

**Controller Builds JSON (TourController.cpp, lines 57-72):**
```cpp
crow::json::wvalue response;
response["success"] = true;
response["totalDistance"] = tour.totalDistance;
response["campusCount"] = (int)tour.stops.size();

int i = 0;
for (const auto &stop : tour.stops) {
    response["route"][i]["campus"] = stop.campus;
    response["route"][i]["distanceFromPrevious"] = stop.distanceFromPrevious;
    i++;
}
return crow::response(200, response);
```

**JSON Response:**
```json
{
    "success": true,
    "totalDistance": 493.0,
    "campusCount": 4,
    "route": [
        {"campus": "Saddleback College", "distanceFromPrevious": 0.0},
        {"campus": "University of California, Irvine (UCI)", "distanceFromPrevious": 16.0},
        {"campus": "University of California, Los Angeles (UCLA)", "distanceFromPrevious": 51.0},
        {"campus": "Arizona State University", "distanceFromPrevious": 426.0}
    ]
}
```

---

### Step 5: Frontend Displays Results

**Frontend (SaddlebackStart.jsx, lines 124-127):**
```javascript
if (response.data.success) {
    setTourResult(response.data);
}
```

**State After Update:**
```javascript
tourResult = {
    success: true,
    totalDistance: 493.0,
    campusCount: 4,
    route: [
        {campus: "Saddleback College", distanceFromPrevious: 0},
        {campus: "UCI", distanceFromPrevious: 16},
        {campus: "UCLA", distanceFromPrevious: 51},
        {campus: "Arizona State University", distanceFromPrevious: 426}
    ]
}
```

**UI Renders (SaddlebackStart.jsx, lines 287-314):**
```jsx
<div className="tour-summary">
    <h2>Your Optimal Tour Route</h2>
    <p>Total Distance: {tourResult.totalDistance.toFixed(2)} miles</p>
    <p>Campuses to Visit: {tourResult.campusCount}</p>
</div>

<ol className="route-list">
    {tourResult.route.map((stop, index) => (
        <li key={index}>
            <div>{stop.campus}</div>
            {stop.distanceFromPrevious > 0 && (
                <div>{stop.distanceFromPrevious.toFixed(2)} miles from previous</div>
            )}
        </li>
    ))}
</ol>
```

**What User Sees:**
```
Your Optimal Tour Route
-----------------------
Total Distance: 493.00 miles
Campuses to Visit: 4

Route Details:
1. Saddleback College
2. University of California, Irvine (UCI)
   16.00 miles from previous stop
3. University of California, Los Angeles (UCLA)
   51.00 miles from previous stop
4. Arizona State University
   426.00 miles from previous stop
```

---

## Summary Diagram

```
+------------------+     HTTP Request      +------------------+
|                  | ------------------->  |                  |
|    FRONTEND      |                       |     BACKEND      |
|    (React)       |                       |     (C++/Crow)   |
|                  | <-------------------  |                  |
+------------------+     JSON Response     +------------------+
        |                                          |
        v                                          v
+------------------+                       +------------------+
|   Component      |                       |   Controller     |
|   State Update   |                       |   (parse/build)  |
+------------------+                       +------------------+
        |                                          |
        v                                          v
+------------------+                       +------------------+
|   UI Re-render   |                       |   TourPlanner    |
|   (show results) |                       |   (algorithm)    |
+------------------+                       +------------------+
                                                   |
                                                   v
                                           +------------------+
                                           |   QueryData      |
                                           |   (SQL queries)  |
                                           +------------------+
                                                   |
                                                   v
                                           +------------------+
                                           |   SQLite DB      |
                                           |   (distances.db) |
                                           +------------------+
```

---

## Key Files Reference

| Layer | File | Purpose |
|-------|------|---------|
| Frontend | `SaddlebackStart.jsx` | UI component, state management |
| Frontend | `axios` calls | HTTP requests to backend |
| Backend | `main.cpp` | Server setup, route registration |
| Backend | `RouteRegister.h` | Maps URLs to controllers |
| Backend | `TourController.cpp` | Handles /api/Tour/* requests |
| Backend | `UniversityController.cpp` | Handles /api/University/* requests |
| Backend | `TourPlanner.cpp` | Tour calculation algorithms |
| Backend | `Queries.cpp` | Database query functions |
| Database | `distances.db` | Campus distance data |
| Database | `souvenirs.db` | Souvenir items and prices |
