# Quick Testing Guide

## Start the Application

**Terminal 1 - Backend:**
```bash
cd /Users/blakeallard/CS1D---Project-1-College-Tour
./build/execBinary
```

**Terminal 2 - Frontend:**
```bash
cd /Users/blakeallard/CS1D---Project-1-College-Tour/frontend
npm run dev
```

Visit: `http://localhost:5173`

---

## Test Souvenir Purchase Flow

1. **Navigate** to ASU Tour page
2. **Enter your name** (e.g., "John Doe")
3. **Select campuses:**
   - UCI
   - UCLA
   - Northwestern
4. **Click** "Calculate Tour"
5. **View the optimal route:**
   ```
   Arizona State University (START)
   → UCI (+375.0 miles)
   → UCLA (+51.0 miles)
   → Northwestern (+2042.5 miles)
   ```
6. **Browse souvenirs** for each campus
7. **Enter quantities:**
   - ASU: T-Shirt × 2
   - UCI: Hoodie × 1
   - UCLA: Keychain × 3
8. **View totals:**
   - Campus totals displayed for each
   - Grand Total shown at bottom
9. **Click** "Complete Purchase"
10. **Check confirmation** with total amount

---

## Test Algorithm Verification

### Option 1: Enable Logging in Code

Edit `src/Controllers/src/TourController.cpp`:

```cpp
// Line 55: Add true for logging
TourResult tour = TourPlanner::calculateOptimalTour(
    startCampus, 
    selectedCampuses,
    true  // <-- Enable logging
);
```

Rebuild:
```bash
cmake --build build
./build/execBinary
```

### Option 2: View Console Output

The backend terminal will show:

```
========================================
TOUR ALGORITHM VERIFICATION
========================================
Starting campus: Arizona State University
Campuses to visit: 3

[ALGORITHM] Current campus: Arizona State University
[ALGORITHM] Distances checked:
  UCI = 375.0 miles
  UCLA = 426.0 miles
  Northwestern = 2043.0 miles
[ALGORITHM] Nearest neighbor: UCI (375.0 miles)

... (continues) ...

VERIFICATION RESULT: PASSED
========================================
```

---

## Test API Endpoints Directly

### Get Souvenirs
```bash
curl http://localhost:18080/api/Souvenir/Arizona%20State%20University
```

Expected:
```json
[
  {"name":"Football Jersey","price":112.44},
  {"name":"Poster","price":9.43},
  {"name":"Sweatshirt","price":42.79}
]
```

### Calculate Tour
```bash
curl -X POST http://localhost:18080/api/Tour/calculate \
  -H "Content-Type: application/json" \
  -d '{
    "startCampus": "Arizona State University",
    "selectedCampuses": ["UCI", "UCLA", "Northwestern"]
  }'
```

Expected:
```json
{
  "success": true,
  "totalDistance": 2468.5,
  "campusCount": 4,
  "route": [...]
}
```

### Save Purchases
```bash
curl -X POST http://localhost:18080/api/Purchase/save \
  -H "Content-Type: application/json" \
  -d '{
    "tourName": "ASU Tour",
    "purchases": [
      {
        "campus": "Arizona State University",
        "item": "T-Shirt",
        "price": 25.50,
        "quantity": 2
      }
    ]
  }'
```

Expected:
```json
{
  "success": true,
  "message": "Purchases saved successfully"
}
```

---

## Verify Database

### View Purchases
```bash
cd Databases
sqlite3 users.db "SELECT * FROM user_purchases ORDER BY id DESC LIMIT 5;"
```

### Check Souvenirs
```bash
sqlite3 souvenirs.db "SELECT college, item, price FROM souvenirs WHERE college = 'Arizona State University' LIMIT 5;"
```

### Calculate Total Spent
```bash
sqlite3 users.db "SELECT user_id, SUM(price * quantity) as total FROM user_purchases GROUP BY user_id;"
```

---

## Expected Results

### Tour Calculation
- Optimal route displayed
- Distances shown between stops
- Total distance calculated

### Souvenir Display
- All souvenirs loaded for each campus
- Prices displayed correctly
- Quantity inputs functional

### Purchase Totals
- Campus totals calculated correctly
- Grand total updates dynamically
- Purchase button enabled when items selected

### Save to Database
- Purchases inserted into user_purchases table
- Correct user_id, campus, item, price, quantity
- Timestamp automatically added

### Algorithm Verification
- Logs show distance comparisons
- Nearest neighbor confirmed at each step
- Verification PASSED message displayed

---

## Troubleshooting

**Backend not starting:**
```bash
killall execBinary
./build/execBinary
```

**Frontend errors:**
```bash
cd frontend
rm -rf node_modules
npm install
npm run dev
```

**Database locked:**
```bash
cd Databases
lsof users.db
# Kill any processes using the database
```

**Build errors:**
```bash
cd build
rm -rf *
cd ..
cmake . -B build
cmake --build build
```

---

## Success Indicators

- No console errors
- Tour route displays correctly
- Souvenirs load for each campus
- Totals calculate accurately
- Purchase saves successfully
- Backend logs show verification PASSED
- Database contains purchase records
