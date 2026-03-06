# Backend Integration Documentation

## Overview

This document describes the complete backend integration implemented for the College Tour application. All frontend pages have been connected to functional backend APIs with proper error handling and loading states.

---

## Architecture

### Backend (C++)
- **Framework**: Crow (lightweight C++ web framework)
- **Database**: SQLite3 (4 separate databases)
- **Port**: 18080
- **API Style**: RESTful

### Frontend (React)
- **HTTP Client**: Axios
- **State Management**: React Hooks (useState, useEffect)
- **Routing**: React Router v7

---

## Implemented Features

### 1. Tour Planning System

#### **Backend Implementation**

##### TourPlanner Algorithm (`src/Database/headers/TourPlanner.h`)
Implements nearest-neighbor greedy algorithm for tour optimization.

**Big-Oh Analysis: O(n²)**
- For each of n campuses to visit
- Search through remaining unvisited campuses to find nearest
- Results in n × (n-1)/2 comparisons = O(n²)

**Key Functions:**
- `calculateOptimalTour()` - Plans tour visiting specified campuses
- `calculateNearestNCampuses()` - Visits N nearest campuses from start
- `calculateFullTour()` - Visits all available campuses

##### TourController API (`src/Controllers/src/TourController.cpp`)

**Endpoints:**

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/Tour/saddleback` | Standard 11-campus tour from Saddleback |
| GET | `/api/Tour/uci` | 13-campus tour from UCI |
| GET | `/api/Tour/asu-{count}` | Tour from ASU visiting {count} campuses |
| GET | `/api/Tour/campuses` | Get all available campuses |
| POST | `/api/Tour/custom` | Custom tour with user-selected campuses |

**Response Format:**
```json
{
  "tourType": "Saddleback Tour",
  "startCampus": "Saddleback College",
  "totalDistance": 425.5,
  "campusCount": 11,
  "stops": [
    {
      "campus": "Saddleback College",
      "distanceFromPrevious": 0,
      "order": 1
    },
    {
      "campus": "University of California, Irvine (UCI)",
      "distanceFromPrevious": 15.2,
      "order": 2
    }
  ]
}
```

#### **Frontend Implementation**

##### Saddleback Tour (`frontend/src/pages/Tours/TourOptions/SaddlebackStart/SaddlebackStart.jsx`)
- Automatically loads optimal tour on page load
- Displays route with distances
- Shows loading state during calculation
- Error handling for failed API calls

##### ASU Tour (`frontend/src/pages/Tours/TourOptions/ASUStart/ASUStart.jsx`)
- User selects number of campuses (1-15)
- Generates tour on button click
- Interactive campus count selector
- Real-time tour generation

##### UCI Tour (`frontend/src/pages/Tours/TourOptions/UCIStart/UCIStart.jsx`)
- Similar to Saddleback tour
- Starts from UCI instead
- Visits all 13 available campuses

##### Custom Tour (`frontend/src/pages/Tours/TourOptions/CustomTour/CustomTour.jsx`)
- Dropdown to select starting campus
- Checkboxes for campus selection
- Validates at least one campus selected
- POST request with custom tour data

---

### 2. Admin System

#### **Backend Implementation**

##### AdminController API (`src/Controllers/src/AdminController.cpp`)

**Authentication:**
- Hardcoded credentials (for development)
- Username: `admin`
- Password: `admin123`

**Endpoints:**

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/Admin/login?username=...&password=...` | Authenticate admin |
| GET | `/api/Admin/campuses` | Get all campuses |
| GET | `/api/Admin/souvenirs-{campus}` | Get souvenirs for campus |
| POST | `/api/Admin/souvenir` | Add new souvenir |
| PATCH | `/api/Admin/souvenir` | Update souvenir price |
| DELETE | `/api/Admin/souvenir-{campus}-{item}` | Delete souvenir |

**Add Souvenir Request:**
```json
{
  "campus": "Saddleback College",
  "item": "T-Shirt",
  "price": 25.99
}
```

**Update Price Request:**
```json
{
  "campus": "Saddleback College",
  "item": "T-Shirt",
  "price": 29.99
}
```

#### **Frontend Implementation**

##### AdminLogin Component (`frontend/src/components/AdminLogin/AdminLogin.jsx`)
- Modal login form
- API authentication against backend
- Session storage for auth state
- Error messages for invalid credentials
- Password field (type="password")
- Enter key support

##### Admin Page (`frontend/src/pages/Admin/Admin.jsx`)
- Protected route with authentication check
- Shows login modal if not authenticated
- Displays username and logout button
- Session-based authentication

##### Add Souvenir (`frontend/src/pages/Admin/AdminOperations/AddSvnr/AddSvnr.jsx`)
- Campus dropdown selection
- Form for item name and price
- Validation for required fields
- Success/error messages
- Form clears on success

##### Edit Souvenir (`frontend/src/pages/Admin/AdminOperations/EditSvnr/EditSvnr.jsx`)
- Campus selector
- Table of existing souvenirs
- Click "Edit" to modify price
- Inline edit form
- Local state updates on success

##### Delete Souvenir (`frontend/src/pages/Admin/AdminOperations/DeleteSvnr/DeleteSvnr.jsx`)
- Campus selector
- Table of existing souvenirs
- Confirmation dialog before delete
- Warning about irreversible action
- Removes from table on success

##### Import Data (`frontend/src/pages/Admin/AdminOperations/ImportData./ImportData.jsx`)
- Informational page
- Explains import process
- Placeholder for future file upload feature

---

## Error Handling

### Backend
- Try-catch blocks in all controllers
- JSON error responses
- Validation before database operations
- Null/empty checks

**Example Error Response:**
```json
{
  "error": "Tour calculation failed",
  "message": "Starting campus not found"
}
```

### Frontend
- Loading states for all async operations
- Error state management with useState
- User-friendly error messages
- Network error handling
- Console logging for debugging

**Loading States:**
```jsx
{loading && <div className="loading-container">Calculating...</div>}
```

**Error States:**
```jsx
{error && <div className="error-message">{error}</div>}
```

---

## Database Schema

### Existing Tables

**distances** (Databases/distances.db)
- `starting_college` TEXT
- `ending_college` TEXT
- `distance` INTEGER

**souvenirs** (Databases/souvenirs.db)
- `college` TEXT
- `item` TEXT
- `price` INTEGER

**users** (Databases/users.db)
- `id` INTEGER PRIMARY KEY
- `username` TEXT
- `password` TEXT
- `wallet_balance` REAL

**user_purchases** (Databases/users.db)
- `id` INTEGER PRIMARY KEY
- `user_id` INTEGER
- `campus` TEXT
- `item` TEXT
- `price` REAL
- `quantity` INTEGER
- `purchase_date` TEXT

---

## Build Instructions

### Backend

1. **Build the C++ backend:**
```bash
cmake . -B build
cmake --build build
```

2. **Run the server:**
```bash
./build/execBinary
```
Server runs on http://localhost:18080

### Frontend

1. **Install dependencies:**
```bash
cd frontend
npm install
```

2. **Development mode:**
```bash
npm run dev
```

3. **Production build:**
```bash
npm run build
```
Built files output to `frontend/dist/` and are automatically served by the C++ backend.

---

## API Testing

### Using curl

**Test Saddleback Tour:**
```bash
curl http://localhost:18080/api/Tour/saddleback
```

**Test Admin Login:**
```bash
curl "http://localhost:18080/api/Admin/login?username=admin&password=admin123"
```

**Test Add Souvenir:**
```bash
curl -X POST http://localhost:18080/api/Admin/souvenir \
  -H "Content-Type: application/json" \
  -d '{"campus":"Saddleback College","item":"Mug","price":15}'
```

**Test Custom Tour:**
```bash
curl -X POST http://localhost:18080/api/Tour/custom \
  -H "Content-Type: application/json" \
  -d '{"startCampus":"Saddleback College","campuses":["UCI","ASU","UCSD"]}'
```

---

## Code Organization

### Backend Structure
```
src/
├── Controllers/
│   ├── headers/
│   │   └── Controllers.h
│   └── src/
│       ├── AdminController.cpp
│       ├── TourController.cpp
│       ├── UniversityController.cpp
│       └── UserController.cpp
├── Database/
│   ├── headers/
│   │   ├── AbsDatabase.h
│   │   ├── DatabaseTypes.h
│   │   ├── Queries.h
│   │   └── TourPlanner.h
│   └── src/
│       ├── AbsDatabase.cpp
│       ├── Queries.cpp
│       └── TourPlanner.cpp
├── RouteRegister.h
└── main.cpp
```

### Frontend Structure
```
frontend/src/
├── components/
│   ├── AdminLogin/
│   │   ├── AdminLogin.jsx
│   │   └── AdminLogin.css
│   ├── HomeButton/
│   └── NavBar/
├── pages/
│   ├── Admin/
│   │   ├── Admin.jsx
│   │   ├── Admin.css
│   │   └── AdminOperations/
│   │       ├── AddSvnr/
│   │       ├── DeleteSvnr/
│   │       ├── EditSvnr/
│   │       └── ImportData/
│   ├── Campuses/
│   │   ├── Campuses.jsx
│   │   └── Campuses.css
│   ├── Home/
│   └── Tours/
│       ├── Tours.jsx
│       └── TourOptions/
│           ├── ASUStart/
│           ├── CustomTour/
│           ├── SaddlebackStart/
│           └── UCIStart/
└── main.jsx
```

---

## Future Enhancements

### Backend
1. **User Management**
   - Implement UserController for purchases and wallet
   - Session management
   - Purchase tracking integration

2. **Advanced Features**
   - File upload for import data
   - Database backup/restore
   - Analytics endpoints

3. **Security**
   - JWT authentication
   - Password hashing (bcrypt)
   - Rate limiting
   - CORS configuration

### Frontend
1. **Souvenir Purchasing**
   - Add purchase interface to tour pages
   - Shopping cart functionality
   - Purchase history

2. **User Experience**
   - Tour visualization (map)
   - Print/export tour itinerary
   - Save custom tours

3. **Admin Dashboard**
   - Statistics and analytics
   - Bulk operations
   - Activity logs

---

## Known Issues / Limitations

1. **Authentication**: Admin credentials are hardcoded (not production-ready)
2. **File Import**: Import Data page is informational only
3. **Validation**: Limited input validation on some forms
4. **Error Recovery**: Some database errors don't provide detailed user feedback
5. **Campus Names**: URL encoding for spaces in campus names (working as intended)

---

## Testing Checklist

### Tour Pages
- [ ] Saddleback tour loads automatically
- [ ] ASU tour accepts campus count input
- [ ] UCI tour displays all campuses
- [ ] Custom tour allows campus selection
- [ ] All tours show loading states
- [ ] Error messages display on API failure

### Admin Pages
- [ ] Login works with correct credentials
- [ ] Login rejects incorrect credentials
- [ ] Add souvenir creates new items
- [ ] Edit souvenir updates prices
- [ ] Delete souvenir removes items
- [ ] Confirmation dialog works
- [ ] Logout clears authentication

### General
- [ ] All pages have HomeButton navigation
- [ ] CSS styling is consistent
- [ ] Loading states are visible
- [ ] Error messages are user-friendly
- [ ] Backend compiles without errors
- [ ] Frontend builds successfully

---

## Contributors

Backend integration implemented with:
- Tour planning algorithms (nearest-neighbor)
- RESTful API endpoints
- Database query optimization
- Error handling and validation
- Frontend-backend communication
- React state management
- Loading and error states
- Admin authentication
- CRUD operations for souvenirs

---

## Support

For issues or questions:
1. Check console logs (browser and server)
2. Verify backend is running on port 18080
3. Ensure databases are in correct location
4. Check network tab for API responses
5. Review error messages in UI

---

**Last Updated**: March 2, 2026
