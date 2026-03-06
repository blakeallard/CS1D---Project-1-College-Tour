# ASU Tour - Recursive Route Optimization Implementation

## Overview

Implemented a complete tour planning system for the ASU Tour page that:
1. Loads available campuses dynamically from the database
2. Calculates the optimal tour route using **recursive backtracking**
3. Displays the full route with distances between each stop

---

## Backend Implementation

### Files Modified/Created:

1. **`src/Controllers/src/TourController.cpp`**
   - Added `POST /api/Tour/calculate` endpoint
   - Accepts: `startCampus` and `selectedCampuses[]`
   - Returns: optimal route with distances

2. **`src/Database/src/Queries.cpp`** & **`src/Database/headers/Queries.h`**
   - Added `double distance(campus1, campus2)` - gets distance between two campuses
   - Added `void allDistances(campus, map)` - gets all distances from a campus

3. **`src/Database/CMakeLists.txt`**
   - Added TourPlanner.cpp and TourPlanner.h to build sources

### Algorithm Used: **Recursive Backtracking (Optimal TSP)**

**Function:** `TourPlanner::calculateOptimalTour()`

**How it works:**
- Explores ALL possible orderings of campuses
- Uses branch-and-bound pruning to skip worse paths
- **Time Complexity:** O(n!) where n = number of campuses
- **Guarantees:** Finds the globally optimal (shortest) route

**Why this is acceptable:**
- Typical tour has 5-10 campuses
- Pruning dramatically reduces search space
- Modern CPUs handle millions of comparisons/second
- Correctness > Speed for educational project

### API Endpoint

**POST** `/api/Tour/calculate`

**Request:**
```json
{
  "startCampus": "Arizona State University",
  "selectedCampuses": ["Northwestern", "UCLA", "UCI"]
}
```

**Response:**
```json
{
  "success": true,
  "totalDistance": 2468.5,
  "campusCount": 4,
  "route": [
    {
      "campus": "Arizona State University",
      "distanceFromPrevious": 0
    },
    {
      "campus": "UCI",
      "distanceFromPrevious": 375
    },
    {
      "campus": "UCLA", 
      "distanceFromPrevious": 51
    },
    {
      "campus": "Northwestern",
      "distanceFromPrevious": 2042.5
    }
  ]
}
```

---

## Frontend Implementation

### File: `frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.jsx`

**Features:**
1. **Campus Loading** - Fetches from `/api/University/all`
2. **Checkbox Selection** - User selects which campuses to visit
3. **Route Calculation** - Calls `/api/Tour/calculate` on submit
4. **Route Display** - Shows ordered stops with distances
5. **Reset Functionality** - Plan another tour button

**Component States:**
- `name` - User's name
- `campuses` - All available campuses
- `selectedCampuses` - User's selections (object: {campusName: bool})
- `tourResult` - Calculated route from backend
- `loading` - Campus loading state
- `calculating` - Tour calculation state
- `error` - Error messages

**Flow:**
1. Page loads → Fetch campuses
2. User enters name + selects campuses
3. Click "Calculate Tour"
4. Backend calculates optimal route
5. Display results with distances
6. Store in localStorage
7. Option to plan another tour

### File: `frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.css`

**New Styles Added:**
- `.tour-result-container` - Results display area
- `.tour-summary` - Summary with total distance
- `.tour-route` - Route list container
- `.route-list` - Ordered list styling
- `.route-stop` - Individual stop styling with numbered circles
- `.stop-campus` - Campus name styling
- `.stop-distance` - Distance info styling

---

## Data Storage

**LocalStorage** (browser-side):
```javascript
{
  name: "John Doe",
  tourType: "ASU Tour",
  selectedCampuses: ["Northwestern", "UCLA"],
  tourResult: { /* full API response */ },
  date: "2026-03-05T..."
}
```

Stored in: `localStorage.getItem('asuTourRegistrations')`

---

## Testing

### Start Backend:
```bash
cd /Users/blakeallard/CS1D---Project-1-College-Tour
./build/execBinary
```

### Start Frontend (Dev):
```bash
cd frontend
npm run dev
```

### Test Flow:
1. Navigate to ASU Tour page
2. Enter your name
3. Select 2-3 campuses
4. Click "Calculate Tour"
5. View the optimal route with distances

### Test API Directly:
```bash
curl -X POST http://localhost:18080/api/Tour/calculate \
  -H "Content-Type: application/json" \
  -d '{
    "startCampus": "Arizona State University",
    "selectedCampuses": ["Northwestern", "UCLA", "UCI"]
  }'
```

---

## Example Output

**Starting Point:** Arizona State University  
**Selected Campuses:** Northwestern, UCLA, UCI

**Optimal Route Calculated:**
1. Arizona State University (starting point)
2. UCI - 375.0 miles from previous stop
3. UCLA - 51.0 miles from previous stop  
4. Northwestern - 2042.5 miles from previous stop

**Total Distance:** 2468.5 miles

---

## Technical Details

### Recursive Algorithm (from TourPlanner.cpp):

```cpp
void findOptimalTourRecursive(
    currentCampus, remainingCampuses, currentDistance, 
    currentPath, bestDistance, bestPath)
{
    // BASE CASE: All campuses visited
    if (remainingCampuses.empty()) {
        if (currentDistance < bestDistance) {
            bestDistance = currentDistance;
            bestPath = currentPath;
        }
        return;
    }
    
    // PRUNING: Skip if already worse than best
    if (currentDistance >= bestDistance) return;
    
    // RECURSIVE: Try each remaining campus
    for (each campus in remainingCampuses) {
        distance = getDistance(currentCampus, campus);
        newPath = currentPath + campus;
        newRemaining = remainingCampuses - campus;
        
        // RECURSE
        findOptimalTourRecursive(
            campus, newRemaining, 
            currentDistance + distance,
            newPath, bestDistance, bestPath
        );
    }
}
```

**Key Features:**
- Exhaustive search guarantees optimal solution
- Branch-and-bound pruning improves performance
- Backtracking handled automatically via function returns

---

## Files Changed Summary

**Backend:**
- ✅ `src/Controllers/src/TourController.cpp` - Added calculate endpoint
- ✅ `src/Database/src/Queries.cpp` - Added distance helper functions  
- ✅ `src/Database/headers/Queries.h` - Added function declarations
- ✅ `src/Database/CMakeLists.txt` - Added TourPlanner to build
- ✅ Uses existing `TourPlanner.cpp` and `TourPlanner.h`

**Frontend:**
- ✅ `frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.jsx` - Full rewrite
- ✅ `frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.css` - Added result styles

**No Changes:**
- ❌ No database schema changes
- ❌ No new database tables
- ❌ Uses existing `/api/University/all` for campus list
- ❌ Only adds one new endpoint: `/api/Tour/calculate`

---

## Success!

The ASU Tour now:
- ✅ Dynamically loads campuses from database
- ✅ Calculates optimal tour using recursive backtracking
- ✅ Displays complete route with distances
- ✅ Shows total distance
- ✅ Stores registrations in localStorage
- ✅ Provides excellent user experience

**Algorithm:** Recursive backtracking with pruning  
**Optimality:** Guaranteed to find shortest route  
**Performance:** Fast for typical 5-10 campus tours
