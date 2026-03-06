# Tour Refactor: ASU and Saddleback Tours

## Date: March 3, 2026

## Summary of Changes

Refactored ASU Tour and Saddleback Tour to allow campus selection with simple greedy routing instead of automatic "visit all campuses" behavior.

## What Changed

### 1. **Backend: New Greedy Algorithm**

**File:** `src/Database/src/TourPlanner.cpp`

Added `calculateGreedyTour()` method:
- **Algorithm:** Greedy nearest-neighbor (deterministic)
- **Time Complexity:** O(n²) where n = selected campuses
- **Behavior:** At each step, picks the closest unvisited campus
- **No TSP:** Does not evaluate all permutations
- **No numeric_limits::max():** Returns valid distances or empty route
- **Validation:** Handles empty selections and missing distances gracefully

```cpp
TourResult TourPlanner::calculateGreedyTour(
    const std::string &startCampus,
    const std::vector<std::string> &selectedCampuses)
{
    // Simple greedy: pick nearest unvisited campus at each step
    // Deterministic: same input → same output
    // No factorial complexity
}
```

### 2. **Backend: Updated API Endpoints**

**File:** `src/Controllers/src/TourController.cpp`

**New POST Endpoints:**
- `POST /api/Tour/custom` with `{tourType: "asu", selectedCampuses: [...]}`
- `POST /api/Tour/custom` with `{tourType: "saddleback", selectedCampuses: [...]}`
- `POST /api/Tour/custom` with `{startCampus: "...", campuses: [...]}` (existing, uses optimal)

**Response Format:**
```json
{
  "success": true,
  "tourType": "ASU Tour",
  "startCampus": "Arizona State University",
  "totalDistance": 457.0,
  "campusCount": 4,
  "stops": [
    {
      "order": 1,
      "campus": "Arizona State University",
      "distanceFromPrevious": 0
    },
    ...
  ]
}
```

### 3. **Frontend: Campus Selection UI**

**Files:**
- `frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.jsx`
- `frontend/src/pages/Tours/TourOptions/SaddlebackStart/SaddlebackStart.jsx`

**Changes:**
- ✅ Removed "visit all campuses" behavior
- ✅ Added checkbox list for campus selection
- ✅ Changed button text from "Calculating optimal tour..." to "Generating route..."
- ✅ Fixed starting campus display
- ✅ Only sends selected campuses to backend
- ✅ Validates at least one campus selected
- ✅ Filters out starting campus from selection list

**New UI Elements:**
```jsx
<div className="campus-selection">
    <h2>Select Campuses to Visit</h2>
    <p className="selection-hint">Starting campus: Arizona State University</p>
    
    <div className="campus-checkboxes">
        {availableCampuses.map((campus) => (
            <label>
                <input type="checkbox" ... />
                <span>{campus}</span>
            </label>
        ))}
    </div>
    
    <button>
        {loading ? "Generating route..." : "Generate Tour"}
    </button>
</div>
```

### 4. **CSS Updates**

**Files:**
- `frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.css`
- `frontend/src/pages/Tours/TourOptions/SaddlebackStart/SaddlebackStart.css`

Added styles for:
- `.campus-selection` - Container for selection UI
- `.campus-checkboxes` - Grid layout for checkboxes
- `.campus-checkbox-label` - Individual checkbox styling
- `.generate-tour-btn` - Tour generation button
- `.selection-hint` - Starting campus indicator

All styling matches existing minimalist dark theme.

## Algorithm Comparison

### Before: TSP Optimal (Removed from ASU/Saddleback)
```cpp
// Recursive backtracking
// O(n!) complexity
// Evaluates all permutations
// Returns numeric_limits<double>::max() on failure
// Used for "visit all campuses"
```

### After: Greedy Nearest-Neighbor (ASU/Saddleback)
```cpp
// Iterative greedy
// O(n²) complexity
// Picks nearest at each step
// Returns valid distance or 0.0
// User selects specific campuses
```

### Still Used: TSP Optimal (Custom Tour Only)
```cpp
// Custom Tour page still uses optimal recursive algorithm
// Guarantees shortest route
// Appropriate for user-curated campus lists
```

## Key Requirements Met

✅ **Fixed starting campus**
- ASU Tour → "Arizona State University"
- Saddleback Tour → "Saddleback College"

✅ **Campus selection**
- Checkbox list of all available campuses
- User selects any subset
- Starting campus excluded from list

✅ **Removed over-engineering**
- No factorial TSP for simple tours
- No numeric_limits::max()
- No "visit all campuses" automatic behavior
- No random selection

✅ **Greedy algorithm**
- Simple, deterministic
- Efficient O(n²)
- Picks nearest unvisited campus

✅ **Updated messaging**
- "Generating route..." instead of "Calculating optimal tour..."
- "Select campuses to visit" instead of "Visit all campuses"
- Clear starting campus indication

✅ **Validation**
- Empty selection returns starting campus only
- Missing distances handled gracefully
- Error messages for invalid requests

✅ **Response format**
```json
{
  "success": true,
  "totalDistance": <number>,
  "route": [...]
}
```

## Testing Results

### Test 1: ASU Tour with 3 Campuses
```bash
curl -X POST http://localhost:18080/api/Tour/custom \
  -H "Content-Type: application/json" \
  -d '{
    "tourType": "asu",
    "selectedCampuses": ["UCI", "UCLA", "Saddleback College"]
  }'
```

**Result:**
```json
{
  "success": true,
  "totalDistance": 457,
  "route": ["ASU", "UCI", "Saddleback", "UCLA"]
}
```

**Greedy logic:**
1. Start: ASU
2. Nearest: UCI (375 miles)
3. Nearest: Saddleback (16 miles from UCI)
4. Nearest: UCLA (66 miles from Saddleback)

✅ Deterministic, efficient, no TSP

### Test 2: Saddleback Tour with 3 Campuses
```bash
curl -X POST http://localhost:18080/api/Tour/custom \
  -H "Content-Type: application/json" \
  -d '{
    "tourType": "saddleback",
    "selectedCampuses": ["ASU", "Northwestern", "Ohio State"]
  }'
```

**Result:**
```json
{
  "success": true,
  "totalDistance": 2470,
  "route": ["Saddleback", "ASU", "Northwestern", "Ohio State"]
}
```

✅ Fixed start, greedy routing, valid distances

### Test 3: Custom Tour Still Optimal
```bash
curl -X POST http://localhost:18080/api/Tour/custom \
  -H "Content-Type: application/json" \
  -d '{
    "startCampus": "ASU",
    "campuses": ["UCI", "UCLA", "Northwestern"]
  }'
```

**Result:**
```json
{
  "success": true,
  "totalDistance": 2469
}
```

✅ Custom Tour still uses optimal recursive algorithm

## Behavioral Changes

### ASU Tour

**Before:**
- Automatically visited all 11 campuses
- Used TSP algorithm
- No user control over destinations
- Showed "Calculating optimal tour..."
- GET `/api/Tour/asu-<count>` endpoint

**After:**
- User selects specific campuses
- Uses greedy nearest-neighbor
- Full control over which campuses to visit
- Shows "Generating route..."
- POST `/api/Tour/custom` with `tourType: "asu"`

### Saddleback Tour

**Before:**
- Automatically visited all 11 campuses
- Used TSP algorithm
- No user control
- Showed "Calculating optimal tour route..."
- Auto-loaded on page mount

**After:**
- User selects specific campuses
- Uses greedy nearest-neighbor
- Full control over destinations
- Shows "Generating route..."
- Loads after user clicks "Generate Tour"

### Custom Tour (Unchanged)

**Still uses optimal recursive TSP:**
- User selects starting campus AND destination campuses
- Evaluates all permutations
- Returns provably optimal route
- Appropriate for this use case

## Files Modified

### Backend
1. `src/Database/headers/TourPlanner.h` - Added `calculateGreedyTour()` declaration
2. `src/Database/src/TourPlanner.cpp` - Implemented greedy algorithm
3. `src/Controllers/src/TourController.cpp` - Added POST handlers for ASU/Saddleback

### Frontend
4. `frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.jsx` - Campus selection UI
5. `frontend/src/pages/Tours/TourOptions/SaddlebackStart/SaddlebackStart.jsx` - Campus selection UI
6. `frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.css` - Selection UI styles
7. `frontend/src/pages/Tours/TourOptions/SaddlebackStart/SaddlebackStart.css` - Selection UI styles

## Deployment

### Build Backend
```bash
cd /Users/blakeallard/CS1D---Project-1-College-Tour
cmake --build build
```

### Build Frontend
```bash
cd frontend
npm run build
```

### Restart Server
```bash
lsof -ti :18080 | xargs kill -9
./build/execBinary
```

### Test in Browser
1. Navigate to http://localhost:18080
2. Click "ASU Tour" or "Saddleback Tour"
3. Select campuses using checkboxes
4. Click "Generate Tour"
5. Verify route displays with valid distances

## Status: ✅ COMPLETE

The ASU and Saddleback tours now:
- ✅ Allow campus selection via checkboxes
- ✅ Use simple greedy nearest-neighbor algorithm
- ✅ Have fixed starting campuses
- ✅ Show "Generating route..." instead of "Calculating optimal..."
- ✅ Return deterministic results
- ✅ Handle edge cases gracefully
- ✅ Match existing UI theme

The Custom Tour still uses the optimal recursive algorithm for maximum flexibility.

---

**Date:** March 3, 2026
**Type:** Feature Refactor
**Complexity:** Reduced (removed TSP from ASU/Saddleback)
