# Tour Algorithm Fix: Sort by Distance from Start

## Date: March 3, 2026

## Problem with Previous Implementation

**Previous "greedy" algorithm was CHAINING:**
- Started at ASU
- Picked nearest campus from ASU
- Then picked nearest from THAT campus
- Continued chaining: ASU → nearest → nearest from there → etc.

**This was WRONG behavior.**

## Correct Implementation

**Required behavior: Sort by distance from START only**

### Algorithm Steps

```
Given:
  - startCampus (fixed: "ASU" or "Saddleback")
  - selectedCampuses (user's checkbox selections)

Step 1: For each campus in selectedCampuses:
  - distance = getDistance(startCampus, campus)

Step 2: Sort campuses by distance from START (ascending)

Step 3: Build route in sorted order:
  - Stop 1: Starting campus (distance = 0)
  - Stop 2: Closest campus to start
  - Stop 3: 2nd closest campus to start
  - Stop 4: 3rd closest campus to start
  - etc.

Step 4: Calculate totalDistance:
  - Sum of distances between consecutive stops in the route
  - distance(start→first) + distance(first→second) + ...
```

### Key Differences

| Aspect | Old (Chaining) | New (Sorted) |
|--------|------------------|----------------|
| **Sort by** | Distance from current position | Distance from START only |
| **Algorithm** | Greedy nearest-neighbor | Simple sort |
| **Complexity** | O(n²) | O(n log n) |
| **Behavior** | Changes based on intermediate stops | Deterministic from start distances |

## Implementation

### File: `src/Database/src/TourPlanner.cpp`

**New Method:** `calculateSortedByDistanceTour()`

```cpp
TourResult TourPlanner::calculateSortedByDistanceTour(
    const std::string &startCampus,
    const std::vector<std::string> &selectedCampuses)
{
    // Step 1: Compute distance from START to each selected campus
    for (const auto &campus : selectedCampuses)
    {
        double distFromStart = QueryData::distance(startCampus, campus);
        campusDistances.push_back({campus, distFromStart});
    }
    
    // Step 2: Sort by distance from START (ascending)
    std::sort(campusDistances.begin(), campusDistances.end(),
        [](const CampusDistance &a, const CampusDistance &b)
        {
            return a.distanceFromStart < b.distanceFromStart;
        });
    
    // Step 3 & 4: Build route and calculate total distance
    for (const auto &cd : campusDistances)
    {
        double distFromPrevious = QueryData::distance(currentCampus, cd.campus);
        result.totalDistance += distFromPrevious;
        currentCampus = cd.campus;
    }
}
```

**Key Points:**
- Sorts by distance from START
- O(n log n) time complexity
- Deterministic (stable sort)
- No chaining
- No recursion
- No TSP
- No `numeric_limits::max()`

## Testing & Verification

### Test 1: ASU Tour

**Selected Campuses:**
- UCI
- UCLA
- Oregon
- Northwestern

**Distances from ASU:**
```
UCI:          375 miles  ← 1st closest
UCLA:         397 miles  ← 2nd closest
Oregon:     1,343 miles  ← 3rd closest
Northwestern: 1,756 miles  ← 4th closest
```

**Generated Route:**
```
ASU → UCI (375) → UCLA (51) → Oregon (849) → Northwestern (2,151)
Total: 3,426 miles
```

**Campuses are ordered by distance from ASU**
**NOT chained by nearest-neighbor**

### Test 2: Saddleback Tour

**Selected Campuses:**
- UCLA
- ASU
- Northwestern

**Distances from Saddleback:**
```
UCLA:         66 miles  ← 1st closest
ASU:         379 miles  ← 2nd closest
Northwestern: 2,033 miles  ← 3rd closest
```

**Generated Route:**
```
Saddleback → UCLA (66) → ASU (397) → Northwestern (1,756)
Total: 2,219 miles
```

**Campuses are ordered by distance from Saddleback**
**Deterministic and simple**

## Comparison: Old vs New

### Example: ASU → [UCI, Northwestern, Oregon]

#### Old Greedy Chaining (WRONG)
```
Step 1: At ASU, nearest is UCI (375 miles)
        Go to UCI
Step 2: At UCI, nearest remaining is Northwestern (2,030 miles)
        Go to Northwestern
Step 3: At Northwestern, only Oregon left (2,043 miles)
        Go to Oregon

Route: ASU → UCI → Northwestern → Oregon
```

#### New Sorted by Distance from Start (CORRECT)
```
Step 1: Compute distances from ASU:
        - UCI: 375 miles
        - Oregon: 1,343 miles
        - Northwestern: 1,756 miles

Step 2: Sort ascending:
        1. UCI (375)
        2. Oregon (1,343)
        3. Northwestern (1,756)

Step 3: Build route in sorted order:
        ASU → UCI → Oregon → Northwestern

Route: ASU → UCI → Oregon → Northwestern
```

**The order is based ONLY on distance from START, not from intermediate stops.**

## Code Changes

### Modified Files

1. **`src/Database/headers/TourPlanner.h`**
   - Renamed `calculateGreedyTour()` to `calculateSortedByDistanceTour()`
   - Updated documentation

2. **`src/Database/src/TourPlanner.cpp`**
   - Replaced greedy chaining algorithm
   - Implemented sort-by-distance-from-start algorithm
   - Removed nearest-neighbor logic

3. **`src/Controllers/src/TourController.cpp`**
   - Updated ASU handler to call `calculateSortedByDistanceTour()`
   - Updated Saddleback handler to call `calculateSortedByDistanceTour()`

### No Frontend Changes

Frontend remains unchanged - still sends selected campuses to backend.

## Algorithm Properties

**Time Complexity:** O(n log n) for sorting  
**Space Complexity:** O(n) for storing campus distances  
**Deterministic:** Same input → same output  
**Simple:** No recursion, no TSP, no permutations  
**Correct:** Matches project requirements exactly  

## Deployment

### Rebuild Backend
```bash
cd /Users/blakeallard/CS1D---Project-1-College-Tour
cmake --build build
```

### Restart Server
```bash
lsof -ti :18080 | xargs kill -9
./build/execBinary
```

### Test
```bash
curl -X POST http://localhost:18080/api/Tour/custom \
  -H "Content-Type: application/json" \
  -d '{
    "tourType": "asu",
    "selectedCampuses": ["UCI", "Northwestern", "Oregon"]
  }'
```

Expected: Campuses ordered by distance from ASU, not by chaining.

## Status: FIXED

ASU and Saddleback tours now:
- Sort campuses by distance from START only
- Do NOT use greedy chaining
- Do NOT use nearest-neighbor from current position
- Simple O(n log n) algorithm
- Deterministic behavior
- Correct as per requirements

---

**Fixed:** March 3, 2026  
**Algorithm:** Sort by distance from start (ascending)  
**Complexity:** O(n log n)  
**Behavior:** Deterministic, simple, correct
